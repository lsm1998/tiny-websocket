//
// Created by Administrator on 2023/12/13.
//

#include "core.hpp"
#include "websocket_handler.hpp"

class MyHandler : public WebsocketHandler
{
public:
    void onAccept(const WebsocketRequest &request, const WebsocketConn &conn) override
    {
        std::cout << "客户端：" << request.getQuery("name") << "加入啦" << std::endl;
        conn.setContext("name", request.getQuery("name"));
    }

    void onRead(const WebsocketConn &conn) override
    {
        char buf[MAX_BUF_SIZE];
        while (true)
        {
            size_t len = conn.recvMessage(WebsocketConn::MessageType::BINARY, buf, MAX_BUF_SIZE);
            if (len < MAX_BUF_SIZE)
            {
                buf[len] = '\0';
                break;
            }
        }
        std::cout << "收到消息来自：" << conn.getContext("name") << "的消息" << std::endl;
        std::cout << "消息内容：" << buf << std::endl;

    }

    void onWrite(const WebsocketConn &conn) override
    {
    }

    void onClose(const WebsocketConn &conn) override
    {
        std::cout << "客户端：" << conn.getContext("name") << "下线！" << std::endl;
    }

    ~MyHandler() override = default;
};

void init(TinyWebsocketServer &server)
{
    server.handle("/ws", new MyHandler());
}