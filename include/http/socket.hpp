
#pragma once

#include <cstdint>
#include <unistd.h>

namespace forge::http {

class Socket {
public:
    explicit Socket(int fd) noexcept;
    ~Socket() noexcept;

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    [[nodiscard]] int get() const noexcept;
    [[nodiscard]] bool valid() const noexcept;

    static Socket createTCP();

    void bind(const char* ip, std::uint16_t port);
    void listen(int backlog=128);

    [[nodiscard]] Socket accept();

    [[nodiscard]] ssize_t receive(char* buffer, std::size_t capacity);

    [[nodiscard]] ssize_t send(const char* buffer, std::size_t size);

    void sendAll(const char* buffer, std::size_t size);

private:
    int fd_;
};

}
