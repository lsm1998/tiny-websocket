//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_HANDLER_H
#define TINY_WEBSOCKET_WEBSOCKET_HANDLER_H

#include "websocket_connect.hpp"

class WebsocketHandler
{
public:
    virtual void onAccept(const WebsocketConn &conn) = 0;

    virtual void onRead(const WebsocketConn &conn) = 0;

    virtual void onWrite(const WebsocketConn &conn) = 0;

    virtual void onClose(const WebsocketConn &conn) = 0;

    virtual ~WebsocketHandler() = default;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_HANDLER_H
