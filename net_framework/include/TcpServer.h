#pragma once

#include "Socket.h"
#include "Epoll.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <atomic>
#include <mutex>
#include <cstdint>

class TcpServer {
public:
    TcpServer(const std::string& ip, uint16_t port, size_t thread_count = 4);
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void set_connection_callback(const ConnectionCallback& cb) { connection_callback_ = cb; }
    void set_message_callback(const MessageCallback& cb) { message_callback_ = cb; }

    // 启动服务器（主事件循环）
    void start();

    // 优雅退出
    void stop();

    // 多连接特性：向当前所有在线客户端广播消息
    void broadcast(const std::string& message);

    // 获取当前活跃连接数
    size_t connection_count();

private:
    void handle_new_connection();
    void remove_connection(const TcpConnectionPtr& conn);

    std::string ip_;
    uint16_t port_;

    Socket listen_socket_;
    Epoll epoll_;
    ThreadPool thread_pool_;

    std::atomic<bool> running_;

    // 维护多条连接的核心容器 (C++11 unordered_map + 智能指针)
    std::mutex conn_mutex_;
    std::unordered_map<int, TcpConnectionPtr> connections_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
};
