#include "TcpServer.h"
#include <iostream>
#include <csignal>
#include <sstream>

// 全局静态变量配合信号实现优雅退出
static std::function<void()> g_shutdown_handler;

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        std::cout << "\n[Signal] 捕获退出信号 (" << sig << ")，准备安全退出..." << std::endl;
        if (g_shutdown_handler) {
            g_shutdown_handler();
        }
    }
}

int main() {
    // 实例化服务器：监听 0.0.0.0:8080，开辟 4 个工作线程
    TcpServer server("0.0.0.0", 8080, 4);

    // 绑定优雅关闭处理句柄
    g_shutdown_handler = [&server]() {
        server.stop();
    };
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // 1. 设置连接建立回调 (C++11 Lambda)
    server.set_connection_callback([&server](const TcpConnectionPtr& conn) {
        std::ostringstream oss;
        oss << "=== 欢迎连接高性能网络服务器 (C++11 Framework) ===\r\n"
            << "您的地址: " << conn->peer_ip() << ":" << conn->peer_port() << "\r\n"
            << "当前在线总人数: " << server.connection_count() << "\r\n"
            << "提示: 输入 'count' 查询在线人数，输入 'all:<消息>' 广播消息给所有人。\r\n"
            << "=================================================\r\n";
        conn->send(oss.str());
    });

    // 2. 设置消息接收回调 (维护多连接数据交互)
    server.set_message_callback([&server](const TcpConnectionPtr& conn, Buffer* buf) {
        // 读取客户端发送的所有文本
        std::string msg = buf->retrieve_all_to_string();

        // 去除尾部换行符
        while (!msg.empty() && (msg.back() == '\r' || msg.back() == '\n')) {
            msg.pop_back();
        }

        std::cout << "[收到消息] 来自 [" << conn->peer_ip() << ":" << conn->peer_port() 
                  << "]: " << msg << std::endl;

        // 命令 1: 查询当前维护的连接数
        if (msg == "count") {
            std::string reply = "[系统提示] 当前在线连接数: " + 
                                std::to_string(server.connection_count()) + "\r\n";
            conn->send(reply);
        }
        // 命令 2: 多连接广播功能
        else if (msg.rfind("all:", 0) == 0) {
            std::string broadcast_content = msg.substr(4);
            std::string notice = "[群发广播] [" + conn->peer_ip() + ":" + 
                                 std::to_string(conn->peer_port()) + "]: " + 
                                 broadcast_content + "\r\n";
            server.broadcast(notice);
        }
        // 默认行为: Echo 回显
        else {
            std::string reply = "[Echo 回显] " + msg + "\r\n";
            conn->send(reply);
        }
    });

    // 3. 运行事件循环
    server.start();

    return 0;
}
