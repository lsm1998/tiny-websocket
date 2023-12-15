//
// Created by Administrator on 2023/12/13.
//

#include "core.hpp"
#include "websocket_handler.hpp"
#include "websocket_message.hpp"

class MyHandler : public WebsocketHandler
{
private:
    MyHandler() = default;

public:
    static MyHandler *getInstance()
    {
        static MyHandler handler;
        return &handler;
    }

    class GC
    {
        ~GC()
        {
            delete MyHandler::getInstance();
        }
    };

    static GC gc;

public:
    MyHandler(MyHandler &handler) = delete;

    MyHandler(MyHandler &&handler) = delete;

    void onAccept(const WebsocketRequest &request, const WebsocketConn &conn) override
    {
        std::cout << "客户端：" << request.getQuery("name") << "加入啦" << std::endl;
        conn.setContext("name", request.getQuery("name"));
        // request.param("");
    }

    void onRead(const WebsocketConn &conn) override
    {
        auto message = conn.recvMessage();
        if (message.isClosed())
        {
            std::cout << "客户端：" << conn.getContext("name") << "下线！" << std::endl;
            return;
        }
        std::cout << "收到消息来自：" << conn.getContext("name") << "的消息" << std::endl;
        std::cout << "消息长度：" << message.getLength() << std::endl;
        std::cout << "消息内容：" << message.getData() << std::endl;

        WebsocketMessage sendMessage;
        sendMessage.messageType = MessageType::TEXT;

        std::string reply = "reply " + std::string(message.getData());
        sendMessage.setData(reply.data(), (int64_t) reply.size());
        conn.sendMessage(sendMessage);
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
    server.handle("/ws", MyHandler::getInstance());
}