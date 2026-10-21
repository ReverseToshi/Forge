
#include "forge/http/socket.hpp"

namespace forge::http {

Socket::Socket(int fd) noexcept
    : fd_(fd)
{
}

Socket::~Socket() noexcept
{
    if (fd_ != -1) {
        ::close(fd_);
    }
}

Socket::Socket(Socket&& other) noexcept
    : fd_(other.fd_)
{
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other) {
        if (fd_ != -1) {
            ::close(fd_);
        }

        fd_ = other.fd_;
        other.fd_ = -1;
    }

    return *this;
}

int Socket::get_fd() const noexcept
{
    return fd_;

}

}
