//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_GLOBAL_HPP
#define TINY_WEBSOCKET_GLOBAL_HPP

#include <iostream>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <cstring>

constexpr int MAX_BUF_SIZE = 1024 * 4;

constexpr int OK = 1;
constexpr int FAIL = -1;

// 101: "Protocols",
constexpr const char *VERSION = "HTTP/1.1";

constexpr const char *BLANK = " ";

constexpr const char *SEPARATOR = "\r\n";

constexpr const char *COLON = ":";

#ifdef __linux__
#define LINUX 1
#elif __APPLE__
#define MACOS 1
#else
#define WINDOWS 1
#endif

#endif //TINY_WEBSOCKET_GLOBAL_HPP
