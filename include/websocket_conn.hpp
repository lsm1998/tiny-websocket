//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_CONN_H
#define TINY_WEBSOCKET_WEBSOCKET_CONN_H

#include "global.hpp"

class WebsocketConn
{
    enum MessageType
    {
        TEXT = 1,
        BINARY = 2,
        CLOSE = 8,
        PING = 9,
        PONG = 10
    };

public:
    virtual size_t sendMessage(MessageType type,char *buf, size_t len) = 0;

    virtual size_t recvMessage(MessageType type, char *buf, size_t len) = 0;

    virtual ~WebsocketConn() = 0;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_CONN_H
