
#include <gtest/gtest.h>

#include "http/socket.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <utility>
#include <sys/socket.h>

TEST(SocketTest, MoveConstructorTransfersOwnership)
{
    int fds[2];

    ASSERT_EQ(::pipe(fds), 0);

    forge::http::Socket original{fds[0]};
    forge::http::Socket moved{std::move(original)};

    EXPECT_EQ(original.get(), -1);
    EXPECT_EQ(moved.get(), fds[0]);

    ::close(fds[1]);
}

TEST(SocketTest, DestructorClosesDescriptor)
{
    int fds[2];

    ASSERT_EQ(::pipe(fds), 0);

    const int readFd = fds[0];
    const int writeFd = fds[1];

    {
        forge::http::Socket socket{readFd};
    }

    errno = 0;

    EXPECT_EQ(::fcntl(readFd, F_GETFD), -1);
    EXPECT_EQ(errno, EBADF);

    ::close(writeFd);
}

TEST(SocketTest, CreatesTcpSocket)
{
    auto socket = forge::http::Socket::createTCP();

    ASSERT_TRUE(socket.valid());

    int type = 0;
    socklen_t length = sizeof(type);

    ASSERT_EQ(
        ::getsockopt(
            socket.get(),
            SOL_SOCKET,
            SO_TYPE,
            &type,
            &length
        ),
        0
    );

    EXPECT_EQ(type, SOCK_STREAM);
}