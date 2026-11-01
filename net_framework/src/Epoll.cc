#include "Epoll.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

Epoll::Epoll(int max_events)
    : epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(max_events) {
    if (epoll_fd_ < 0) {
        std::cerr << "epoll_create1 failed" << std::endl;
    }
}

Epoll::~Epoll() {
    if (epoll_fd_ >= 0) {
        ::close(epoll_fd_);
    }
}

bool Epoll::add_fd(int fd, uint32_t events) {
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    return ::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == 0;
}

bool Epoll::mod_fd(int fd, uint32_t events) {
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    return ::epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) == 0;
}

bool Epoll::del_fd(int fd) {
    return ::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) == 0;
}

int Epoll::wait(std::vector<struct epoll_event>* active_events, int timeout_ms) {
    int num_events = ::epoll_wait(epoll_fd_, events_.data(), static_cast<int>(events_.size()), timeout_ms);
    if (num_events > 0) {
        active_events->assign(events_.begin(), events_.begin() + num_events);
        // 如果事件填满了当前数组，按需动态扩容
        if (static_cast<size_t>(num_events) == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    }
    return num_events;
}
