
#include <gtest/gtest.h>

#include "forge/http/socket.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <utility>

TEST(SocketTest, MoveConstructorTransfersOwnership)
{
    int fds[2];

    ASSERT_EQ(::pipe(fds), 0);

    forge::http::Socket original{fds[0]};
    forge::http::Socket moved{std::move(original)};

    EXPECT_EQ(original.get_fd(), -1);
    EXPECT_EQ(moved.get_fd(), fds[0]);

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
