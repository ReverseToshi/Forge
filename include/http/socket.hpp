
#pragma once

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

private:
    int fd_;
};

}
