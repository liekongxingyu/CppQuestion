# C++11 高性能网络服务器框架 (NetFramework)

基于现代 **C++11** 与 **Linux Epoll (ET 边缘触发)** 实现的高性能非阻塞 Reactor 架构网络库，支持维护高并发多客户端长连接。

## 一、核心架构设计

```
                    +-----------------------------+
                    |          TcpServer          |
                    |   (监听端口 / 维护所有连接)   |
                    +--------------+--------------+
                                   |
                  +----------------+----------------+
                  |                                 |
         +--------v--------+               +--------v--------+
         |      Epoll      |               |   ThreadPool    |
         |  (ET 边缘触发)   |               |   (C++11 线程池) |
         +--------+--------+               +-----------------+
                  |
         +--------v--------------------------------+
         |            TcpConnection                |
         | (shared_from_this 状态机 / 读写Buffer)   |
         |    - Connection 1 (fd=5)                |
         |    - Connection 2 (fd=6)                |
         |    - Connection N (fd=...)              |
         +-----------------------------------------+
```

## 二、用到的 C++11 新特性

1. **智能指针与生命周期管理**：
   - `std::shared_ptr<TcpConnection>` / `std::weak_ptr`：自动管理客户端连接生命周期，避免悬垂指针。
   - `std::enable_shared_from_this<TcpConnection>`：在异步回调与事件派发时安全获取自身 `shared_ptr`，延长自身生命周期直到事件处理完成。
   - `std::unique_ptr<Socket>`：RAII 严格独占底层描述符资源。
2. **函数对象与回调机制**：
   - `std::function` + `std::bind` 与 **Lambda 表达式**：实现类似 Muduo 的非侵入式业务逻辑解耦（连接回调、读写回调、关闭回调）。
3. **并发与多线程同步**：
   - `std::thread`、`std::mutex`、`std::condition_variable`、`std::unique_lock` 实现工作线程池。
   - `std::atomic<bool>` / `std::atomic<State>`：无锁控制服务器运行状态与连接状态机。
4. **移动语义与性能优化**：
   - 移动构造函数与移动赋值运算符（`Socket(Socket&&)`），禁用裸指针拷贝。
   - `std::move` 减少任务传递时的额外开销。
5. **语法糖与现代类型系统**：
   - `auto`、`nullptr`、`using` 别名定义、`enum class` 强类型枚举、范围 for 循环（`for (auto& conn : connections_)`）。

## 三、文件组织

```
net_framework/
├── include/                       # 存放所有头文件 (.h)
│   ├── Buffer.h                   # 动态自适应应用层读写缓冲区
│   ├── Socket.h                   # Socket RAII 封装
│   ├── Epoll.h                    # Linux Epoll 事件驱动封装
│   ├── ThreadPool.h               # C++11 工业级通用线程池
│   ├── TcpConnection.h            # 单条连接封装，维护状态机、输入输出 Buffer
│   └── TcpServer.h                # 核心服务器类，管理所有连接与广播
├── src/                           # 存放源实现文件 (.cc)
│   ├── Buffer.cc
│   ├── Socket.cc
│   ├── Epoll.cc
│   ├── TcpConnection.cc
│   └── TcpServer.cc
├── main.cc                        # 业务示例：在线人数、Echo 回显、广播群发
├── CMakeLists.txt                 # CMake 编译配置文件
├── README.md                      # 项目说明文档
└── test_clients.py                # 多客户端并发测试脚本
```

## 四、编译与运行

```bash
# 1. 编译
mkdir -p build && cd build
cmake ..
make

# 2. 启动服务器 (监听 8080 端口)
./net_server

# 3. 运行多客户端测试 (另起一个终端)
python3 ../test_clients.py
```
