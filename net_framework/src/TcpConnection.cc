#include "TcpConnection.h"
#include "Epoll.h"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <cerrno>

TcpConnection::TcpConnection(int fd, const std::string& ip, uint16_t port, Epoll* epoll)
    : socket_(new Socket(fd)),
      peer_ip_(ip),
      peer_port_(port),
      epoll_(epoll),
      state_(State::Connecting) {
    socket_->set_non_blocking(true);
    socket_->set_keep_alive(true);
}

TcpConnection::~TcpConnection() {
    // 析构时打印连接释放信息，验证生命周期安全
    std::cout << "[TcpConnection] 释放连接资源 fd=" << socket_->fd()
              << " (" << peer_ip_ << ":" << peer_port_ << ")" << std::endl;
}

void TcpConnection::establish_connection() {
    state_ = State::Connected;
    // 将该连接注册到 Epoll，监听可读和断开事件
    epoll_->add_fd(socket_->fd(), EPOLLIN | EPOLLRDHUP | EPOLLET);
    if (connection_callback_) {
        connection_callback_(shared_from_this());
    }
}

void TcpConnection::destroy_connection() {
    if (state_ == State::Connected) {
        state_ = State::Disconnected;
        epoll_->del_fd(socket_->fd());
        socket_->close();
    }
}

void TcpConnection::send(const std::string& message) {
    send(message.data(), message.size());
}

void TcpConnection::send(const char* data, size_t len) {
    if (state_ != State::Connected) {
        std::cerr << "[TcpConnection] 连接已断开，无法发送数据" << std::endl;
        return;
    }

    ssize_t nwrote = 0;
    size_t remaining = len;
    bool fault_error = false;

    // 如果发送缓冲区为空，尝试直接写到 socket
    if (output_buffer_.readable_bytes() == 0) {
        nwrote = ::write(socket_->fd(), data, len);
        if (nwrote >= 0) {
            remaining = len - nwrote;
        } else {
            nwrote = 0;
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                fault_error = true;
                std::cerr << "[TcpConnection] write error: " << errno << std::endl;
            }
        }
    }

    if (!fault_error && remaining > 0) {
        // 如果未写完，将剩余数据追加到输出缓冲区，并注册 EPOLLOUT 事件
        output_buffer_.append(data + nwrote, remaining);
        epoll_->mod_fd(socket_->fd(), EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
    }
}

void TcpConnection::handle_read() {
    int saved_errno = 0;
    // ET 边缘触发模式下，循环读取直到 EAGAIN
    while (true) {
        ssize_t n = input_buffer_.read_fd(socket_->fd(), &saved_errno);
        if (n > 0) {
            if (message_callback_) {
                message_callback_(shared_from_this(), &input_buffer_);
            }
        } else if (n == 0) {
            // 对端正常关闭连接
            handle_close();
            break;
        } else {
            if (saved_errno == EAGAIN || saved_errno == EWOULDBLOCK) {
                // 读缓冲区已清空
                break;
            } else if (saved_errno == EINTR) {
                // 被信号中断，继续读
                continue;
            } else {
                handle_error();
                break;
            }
        }
    }
}

void TcpConnection::handle_write() {
    if (output_buffer_.readable_bytes() > 0) {
        ssize_t n = ::write(socket_->fd(), output_buffer_.peek(), output_buffer_.readable_bytes());
        if (n > 0) {
            output_buffer_.retrieve(n);
            // 如果全部写完，移除 EPOLLOUT 关注，防止 busy loop
            if (output_buffer_.readable_bytes() == 0) {
                epoll_->mod_fd(socket_->fd(), EPOLLIN | EPOLLRDHUP | EPOLLET);
            }
        } else if (n < 0 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
            handle_error();
        }
    }
}

void TcpConnection::handle_close() {
    if (state_ == State::Connected || state_ == State::Disconnecting) {
        state_ = State::Disconnected;
        epoll_->del_fd(socket_->fd());

        // 保留自身引用，通知服务器端移除连接
        TcpConnectionPtr guard_this(shared_from_this());
        if (close_callback_) {
            close_callback_(guard_this);
        }
    }
}

void TcpConnection::handle_error() {
    std::cerr << "[TcpConnection] 发生异常错误，强制关闭连接 fd=" << socket_->fd() << std::endl;
    handle_close();
}
