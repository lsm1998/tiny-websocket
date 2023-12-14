//
// Created by Administrator on 2023/12/13.
//

#include "core.hpp"
#include "websocket_handler.hpp"

class MyHandler : public WebsocketHandler
{
public:
    void onAccept(const WebsocketRequest& request, const WebsocketConn &conn) override
    {

    }

    void onRead(const WebsocketConn &conn) override
    {

    }

    void onWrite(const WebsocketConn &conn) override
    {

    }

    void onClose(const WebsocketConn &conn) override
    {

    }

    ~MyHandler() override = default;
};

void init(TinyWebsocketServer &server)
{
    server.handle("/ws", new MyHandler());
}