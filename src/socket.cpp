
#include "http/socket.hpp"
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>
#include <stdexcept>
#include <cerrno>

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

    int reuse = 1;
    if (::setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &reuse,
        sizeof(reuse)
    )==-1){
        throw std::system_error(
            errno,
            std::system_category(),
            "setsockopt"
        );
    }
    return Socket(fd);
}

void Socket::bind(const char* ip, std::uint16_t port){
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    const int result = ::inet_pton(
        AF_INET,
        ip,
        &addr.sin_addr
    );

    if(result==0){
        throw std::invalid_argument("Invalid IPv4 address");
    }

    if(result==-1){
        throw std::system_error(
            errno,
            std::system_category(),
            "inet_pton"
        );
    }

    if(::bind(
        fd_,
        reinterpret_cast<const sockaddr*>(&addr),
        sizeof(addr)
    )==-1){
        throw std::system_error(
            errno,
            std::system_category(),
            "bind"
        );
    }
}

void Socket::listen(int backlog){
    if(::listen(fd_, backlog) == -1){
        throw std::system_error(
            errno,
            std::system_category(),
            "listen"
        );
    }
}

Socket Socket::accept(){
    int clientFd = ::accept(fd_, nullptr, nullptr);

    if (clientFd == -1) {
        throw std::system_error(
            errno,
            std::system_category(),
            "accept"
        );
    }

    return Socket(clientFd);
}

ssize_t Socket::receive(char* buffer, std::size_t capacity){
    const ssize_t bytesReceived = ::recv(
        fd_,
        buffer,
        capacity,
        0
    );

    if (bytesReceived == -1) {
        throw std::system_error(
            errno,
            std::system_category(),
            "recv"
        );
    }

    return bytesReceived;
}

ssize_t Socket::send(const char* buffer, std::size_t size){
    const ssize_t bytesSent = ::send(
        fd_,
        buffer,
        size,
        0
    );

    if (bytesSent == -1) {
        throw std::system_error(
            errno,
            std::system_category(),
            "send"
        );
    }

    return bytesSent;
}

void Socket::sendAll(const char* buffer, std::size_t size){
    std::size_t totalSent = 0;

    while(totalSent<size){
        const ssize_t bytesSent = send(
            buffer + totalSent,
            size - totalSent
        );

        if(bytesSent == 0){
            throw std::runtime_error("Socket closed while sending");
        }

        totalSent += static_cast<std::size_t>(bytesSent);
    }
}
}