//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
#define TINY_WEBSOCKET_WEBSOCKET_CONNECT_H

#include "global.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

constexpr char *WEBSOCKET_GUID = (char *) "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

class WebsocketConn
{
public:
    enum MessageType
    {
        TEXT = 1,
        BINARY = 2,
        CLOSE = 8,
        PING = 9,
        PONG = 10
    };

public:
    virtual size_t sendMessage(MessageType type, char *buf, size_t len) = 0;

    virtual size_t recvMessage(MessageType type, char *buf, size_t len) = 0;

    virtual ~WebsocketConn() = default;
};

class WebsocketConnect : public WebsocketConn
{
public:
    explicit WebsocketConnect(int fd) : fd(fd)
    {}

    ~WebsocketConnect() override = default;

    size_t sendMessage(MessageType type, char *buf, size_t len) override;

    size_t recvMessage(MessageType type, char *buf, size_t len) override;

private:
    void checkHandshake();

    bool handshake();

private:
    int fd;

    bool is_handshake{false};

    std::string sec_websocket_key;

    std::string sec_webSocket_extensions;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
