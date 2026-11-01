#pragma once

#include <vector>
#include <sys/epoll.h>

class Epoll {
public:
    explicit Epoll(int max_events = 1024);
    ~Epoll();

    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;

    bool add_fd(int fd, uint32_t events);
    bool mod_fd(int fd, uint32_t events);
    bool del_fd(int fd);

    // 等待 IO 事件就绪，返回就绪事件列表
    int wait(std::vector<struct epoll_event>* active_events, int timeout_ms = -1);

private:
    int epoll_fd_;
    std::vector<struct epoll_event> events_;
};
