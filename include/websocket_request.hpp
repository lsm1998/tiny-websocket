//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_REQUEST_HPP
#define TINY_WEBSOCKET_WEBSOCKET_REQUEST_HPP

#include "http_request.hpp"
#include "http_response.hpp"
#include "base64.hpp"
#include "sha1.hpp"

constexpr char *WEBSOCKET_GUID = (char *) "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

class WebsocketRequest
{
public:
    explicit WebsocketRequest(int fd);

    bool handshake(const std::string &path);

private:
    bool fail(int code, HttpResponse &response);

private:
    int fd;

    HttpRequest request;

    std::string sec_websocket_key;

    std::string sec_webSocket_extensions;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_REQUEST_HPP
