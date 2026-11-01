#include "TcpServer.h"
#include <sys/socket.h>
#include <iostream>
#include <cerrno>

TcpServer::TcpServer(const std::string& ip, uint16_t port, size_t thread_count)
    : ip_(ip),
      port_(port),
      listen_socket_(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0)),
      epoll_(1024),
      thread_pool_(thread_count),
      running_(false) {
    listen_socket_.set_reuse_addr(true);
    listen_socket_.set_reuse_port(true);
}

TcpServer::~TcpServer() {
    stop();
}

void TcpServer::start() {
    if (!listen_socket_.bind_address(ip_, port_)) {
        std::cerr << "[TcpServer] 绑定地址失败 " << ip_ << ":" << port_ << std::endl;
        return;
    }

    if (!listen_socket_.listen(128)) {
        std::cerr << "[TcpServer] 监听端口失败" << std::endl;
        return;
    }

    // 将监听 socket 注册到 Epoll，边缘触发模式
    epoll_.add_fd(listen_socket_.fd(), EPOLLIN | EPOLLET);
    running_ = true;

    std::cout << "==================================================" << std::endl;
    std::cout << " [TcpServer] 启动成功，监听地址: " << (ip_.empty() ? "0.0.0.0" : ip_)
              << ":" << port_ << std::endl;
    std::cout << " [TcpServer] 运行架构: C++11 Reactor + Epoll + ThreadPool" << std::endl;
    std::cout << "==================================================" << std::endl;

    std::vector<struct epoll_event> active_events;

    while (running_) {
        int num_events = epoll_.wait(&active_events, 1000); // 1秒超时，便于响应退出信号
        if (num_events < 0) {
            if (errno == EINTR) continue;
            std::cerr << "[TcpServer] epoll_wait 异常" << std::endl;
            break;
        }

        for (int i = 0; i < num_events; ++i) {
            int fd = active_events[i].data.fd;
            uint32_t events = active_events[i].events;

            if (fd == listen_socket_.fd()) {
                // 有新的客户端连接请求
                handle_new_connection();
            } else {
                // 查找对应的现有连接
                TcpConnectionPtr conn;
                {
                    std::lock_guard<std::mutex> lock(conn_mutex_);
                    auto it = connections_.find(fd);
                    if (it != connections_.end()) {
                        conn = it->second;
                    }
                }

                if (!conn) continue;

                // 优先处理异常/挂断事件
                if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                    conn->handle_close();
                } else {
                    if (events & EPOLLIN) {
                        conn->handle_read();
                    }
                    if (events & EPOLLOUT) {
                        conn->handle_write();
                    }
                }
            }
        }
    }
}

void TcpServer::stop() {
    if (running_) {
        running_ = false;
        std::cout << "[TcpServer] 正在停止服务器并释放所有客户端连接..." << std::endl;
        std::lock_guard<std::mutex> lock(conn_mutex_);
        for (auto& pair : connections_) {
            pair.second->destroy_connection();
        }
        connections_.clear();
        listen_socket_.close();
        std::cout << "[TcpServer] 服务器已完全停止。" << std::endl;
    }
}

void TcpServer::handle_new_connection() {
    // 边缘触发模式下循环 accept 直至读完
    while (true) {
        std::string client_ip;
        uint16_t client_port = 0;
        int client_fd = listen_socket_.accept(&client_ip, &client_port);

        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break; // 所有等待连接已全部取出
            } else if (errno == EINTR) {
                continue;
            } else {
                std::cerr << "[TcpServer] accept4 失败: " << errno << std::endl;
                break;
            }
        }

        // 创建新连接对象，使用 std::make_shared 进行安全管理
        auto conn = std::make_shared<TcpConnection>(client_fd, client_ip, client_port, &epoll_);

        // 设置业务回调
        conn->set_connection_callback(connection_callback_);
        conn->set_message_callback(message_callback_);

        // 绑定断开连接回调，使用 Lambda 捕获 this
        conn->set_close_callback([this](const TcpConnectionPtr& c) {
            this->remove_connection(c);
        });

        {
            std::lock_guard<std::mutex> lock(conn_mutex_);
            connections_[client_fd] = conn;
        }

        // 注册事件并触发连接建立通知
        conn->establish_connection();

        std::cout << "[+] [新建连接] 来自 " << client_ip << ":" << client_port
                  << " (fd=" << client_fd << ")，当前在线连接总数: "
                  << connection_count() << std::endl;
    }
}

void TcpServer::remove_connection(const TcpConnectionPtr& conn) {
    int fd = conn->fd();
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        connections_.erase(fd);
    }
    conn->destroy_connection();

    std::cout << "[-] [连接断开] 来自 " << conn->peer_ip() << ":" << conn->peer_port()
              << " (fd=" << fd << ")，当前在线连接总数: "
              << connection_count() << std::endl;
}

void TcpServer::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(conn_mutex_);
    for (const auto& pair : connections_) {
        pair.second->send(message);
    }
}

size_t TcpServer::connection_count() {
    std::lock_guard<std::mutex> lock(conn_mutex_);
    return connections_.size();
}
