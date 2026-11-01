#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <sys/types.h>

// 现代网络库核心数据结构：动态自适应读写缓冲区
class Buffer {
public:
    static constexpr size_t kInitialSize = 1024;

    explicit Buffer(size_t initial_size = kInitialSize)
        : buffer_(initial_size), read_index_(0), write_index_(0) {}

    // 可读、可写字节数
    size_t readable_bytes() const { return write_index_ - read_index_; }
    size_t writable_bytes() const { return buffer_.size() - write_index_; }

    // 指向可读数据的起始地址
    const char* peek() const { return buffer_.data() + read_index_; }

    // 消费/读取指定长度的数据
    void retrieve(size_t len) {
        if (len < readable_bytes()) {
            read_index_ += len;
        } else {
            retrieve_all();
        }
    }

    void retrieve_all() {
        read_index_ = 0;
        write_index_ = 0;
    }

    std::string retrieve_all_to_string() {
        std::string str(peek(), readable_bytes());
        retrieve_all();
        return str;
    }

    // 追加数据到写缓冲区
    void append(const char* data, size_t len) {
        ensure_writable_bytes(len);
        std::copy(data, data + len, buffer_.data() + write_index_);
        write_index_ += len;
    }

    void append(const std::string& str) {
        append(str.data(), str.size());
    }

    // 从非阻塞 socket 读取数据至缓冲区（通过 readv 分散读避免反复扩容）
    ssize_t read_fd(int fd, int* saved_errno);

private:
    void ensure_writable_bytes(size_t len) {
        if (writable_bytes() < len) {
            make_space(len);
        }
    }

    void make_space(size_t len) {
        // 如果内部腾挪空间也不够，则扩容 vector
        if (writable_bytes() + read_index_ < len) {
            buffer_.resize(write_index_ + len);
        } else {
            // 将已有未读数据挪到最前面
            size_t readable = readable_bytes();
            std::copy(buffer_.data() + read_index_, buffer_.data() + write_index_, buffer_.data());
            read_index_ = 0;
            write_index_ = readable;
        }
    }

    std::vector<char> buffer_;
    size_t read_index_;
    size_t write_index_;
};
