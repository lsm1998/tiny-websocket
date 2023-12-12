//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
#define TINY_WEBSOCKET_WEBSOCKET_CONNECT_H

#include "websocket_conn.hpp"

class WebsocketConnect : public WebsocketConn
{
public:
    explicit WebsocketConnect(int fd) : fd(fd)
    {}

    ~WebsocketConnect() override = 0;

private:
    int fd;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
