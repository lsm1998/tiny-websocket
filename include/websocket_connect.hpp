//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
#define TINY_WEBSOCKET_WEBSOCKET_CONNECT_H

#include "global.hpp"

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
    int fd;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
