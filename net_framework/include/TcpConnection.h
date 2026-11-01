#pragma once

#include "Socket.h"
#include "Buffer.h"
#include <memory>
#include <functional>
#include <atomic>
#include <string>
#include <cstdint>

class Epoll;
class TcpConnection;

// 常用智能指针及回调别名定义 (C++11 using 语法)
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*)>;
using CloseCallback = std::function<void(const TcpConnectionPtr&)>;

// C++11 标准连接抽象类：维护单条客户端连接生命周期
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    enum class State {
        Connecting,
        Connected,
        Disconnecting,
        Disconnected
    };

    TcpConnection(int fd, const std::string& ip, uint16_t port, Epoll* epoll);
    ~TcpConnection();

    TcpConnection(const TcpConnection&) = delete;
    TcpConnection& operator=(const TcpConnection&) = delete;

    int fd() const { return socket_->fd(); }
    const std::string& peer_ip() const { return peer_ip_; }
    uint16_t peer_port() const { return peer_port_; }
    bool is_connected() const { return state_ == State::Connected; }

    void set_connection_callback(const ConnectionCallback& cb) { connection_callback_ = cb; }
    void set_message_callback(const MessageCallback& cb) { message_callback_ = cb; }
    void set_close_callback(const CloseCallback& cb) { close_callback_ = cb; }

    // 连接建立与销毁
    void establish_connection();
    void destroy_connection();

    // 发送数据
    void send(const std::string& message);
    void send(const char* data, size_t len);

    // 事件处理核心入口（由 Epoll 事件驱动）
    void handle_read();
    void handle_write();
    void handle_close();
    void handle_error();

private:
    std::unique_ptr<Socket> socket_;
    std::string peer_ip_;
    uint16_t peer_port_;
    Epoll* epoll_;

    std::atomic<State> state_;
    Buffer input_buffer_;
    Buffer output_buffer_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    CloseCallback close_callback_;
};
