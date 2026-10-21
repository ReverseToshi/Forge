
#include "http/socket.hpp"
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#include <netinet/in.h>
#include <unistd.h>

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

[[nodiscard]] int Socket::get() const noexcept
{
    return fd_;
}

[[nodiscard]] bool Socket::valid() const noexcept
{
    return fd_ != -1;

}

Socket Socket::createTCP()
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        throw std::system_error(
            errno, 
            std::generic_category(), 
            "Failed to create TCP socket"
        );
    }
    return Socket(fd);
}
}