#pragma once

#include <string>
#include <cstdint>

// Socket RAII 封装：遵循 C++11 移动语义，严禁拷贝
class Socket {
public:
    explicit Socket(int fd = -1);
    ~Socket();

    // 禁用拷贝构造与赋值
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // 启用 C++11 移动语义
    Socket(Socket&& rhs) noexcept;
    Socket& operator=(Socket&& rhs) noexcept;

    int fd() const { return fd_; }

    void set_non_blocking(bool on = true);
    void set_reuse_addr(bool on = true);
    void set_reuse_port(bool on = true);
    void set_keep_alive(bool on = true);

    bool bind_address(const std::string& ip, uint16_t port);
    bool listen(int backlog = 128);
    int accept(std::string* client_ip, uint16_t* client_port);

    void close();

private:
    int fd_;
};
