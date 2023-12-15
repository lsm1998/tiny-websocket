//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_BASE64_H
#define TINY_WEBSOCKET_BASE64_H

#include "global.hpp"
#include <algorithm>
#include <array>

std::string base64_encode(unsigned char const *bytes_to_encode, unsigned int in_len);

extern std::string base64_encode(const std::string &input);

extern std::string base64_decode(const std::string &input);

#endif //TINY_WEBSOCKET_BASE64_H
