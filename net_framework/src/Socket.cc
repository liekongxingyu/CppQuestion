#include "Socket.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

Socket::Socket(int fd) : fd_(fd) {}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& rhs) noexcept : fd_(rhs.fd_) {
    rhs.fd_ = -1;
}

Socket& Socket::operator=(Socket&& rhs) noexcept {
    if (this != &rhs) {
        close();
        fd_ = rhs.fd_;
        rhs.fd_ = -1;
    }
    return *this;
}

void Socket::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

void Socket::set_non_blocking(bool on) {
    if (fd_ < 0) return;
    int flags = ::fcntl(fd_, F_GETFL, 0);
    if (on) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    ::fcntl(fd_, F_SETFL, flags);
}

void Socket::set_reuse_addr(bool on) {
    if (fd_ < 0) return;
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Socket::set_reuse_port(bool on) {
    if (fd_ < 0) return;
#ifdef SO_REUSEPORT
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#endif
}

void Socket::set_keep_alive(bool on) {
    if (fd_ < 0) return;
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
}

bool Socket::bind_address(const std::string& ip, uint16_t port) {
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (ip.empty() || ip == "0.0.0.0") {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    }

    return ::bind(fd_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == 0;
}

bool Socket::listen(int backlog) {
    return ::listen(fd_, backlog) == 0;
}

int Socket::accept(std::string* client_ip, uint16_t* client_port) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    std::memset(&client_addr, 0, sizeof(client_addr));

    // 使用 accept4 直接原子设置非阻塞和执行时关闭 (SOCK_NONBLOCK | SOCK_CLOEXEC)
    int client_fd = ::accept4(fd_, reinterpret_cast<struct sockaddr*>(&client_addr), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (client_fd >= 0) {
        if (client_ip) {
            char ip_buf[INET_ADDRSTRLEN] = {0};
            ::inet_ntop(AF_INET, &client_addr.sin_addr, ip_buf, sizeof(ip_buf));
            *client_ip = ip_buf;
        }
        if (client_port) {
            *client_port = ntohs(client_addr.sin_port);
        }
    }
    return client_fd;
}
