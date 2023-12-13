//
// Created by 刘时明 on 2023/12/10.
//

#include "tcp_net_util.hpp"

int netSetBlock(int fd, bool non_block)
{
    int flags;
    /* Set the socket blocking (if non_block is zero) or non-blocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1)
    {
        perror("fcntl(F_GETFL) fail");
        return -1;
    }

    if (non_block)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        perror("fcntl(F_SETFL,O_NONBLOCK) fail");
        return -1;
    }
    return 0;
}

size_t readUntilDelimiter(int fd, char delimiter, char *buf, ssize_t len)
{
    char c;
    size_t result = FAIL;
    for (ssize_t i = 0; i < len; i++)
    {
        ssize_t offset = read(fd, &c, 1);
        if (offset < 0)
        {
            break;
        }
        buf[i] = c;
        if (c == delimiter)
        {
            result = i + 1;
            break;
        }
    }
    return result;
}