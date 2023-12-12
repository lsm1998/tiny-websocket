//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_GLOBAL_HPP
#define TINY_WEBSOCKET_GLOBAL_HPP

#include <iostream>
#include <map>

constexpr int MAX_BUF_SIZE = 1024 * 4;

constexpr int OK = 1;
constexpr int FAIL = -1;

// 101: "Protocols",
constexpr const char *VERSION = "HTTP/1.1";

constexpr const char *BLANK = " ";

constexpr const char *SEPARATOR = "\r\n";

constexpr const char *COLON = ":";

#endif //TINY_WEBSOCKET_GLOBAL_HPP
