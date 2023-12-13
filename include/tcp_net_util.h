//
// Created by 刘时明 on 2023/12/10.
//

#ifndef TINY_HTTP_TCP_NET_UTIL_CPP_H
#define TINY_HTTP_TCP_NET_UTIL_CPP_H

#include "global.hpp"

extern int netSetBlock(int fd, bool non_block);

extern size_t readUntilDelimiter(int fd, char delimiter, char *buf, ssize_t len);

#endif //TINY_HTTP_TCP_NET_UTIL_CPP_H
