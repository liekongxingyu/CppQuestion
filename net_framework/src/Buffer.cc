#include "Buffer.h"
#include <sys/uio.h>
#include <unistd.h>
#include <cerrno>

// 使用 readv 技术，先读满自带 buffer，溢出部分自动落入 64KB 栈空间，然后统一追加
ssize_t Buffer::read_fd(int fd, int* saved_errno) {
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writable_bytes();

    vec[0].iov_base = buffer_.data() + write_index_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);

    if (n < 0) {
        *saved_errno = errno;
    } else if (static_cast<size_t>(n) <= writable) {
        write_index_ += n;
    } else {
        write_index_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;
}
