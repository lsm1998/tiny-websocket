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
        auto name = request.getQuery("name");
        if (this->checkExist(name, conn))
        {
            return;
        }
        connMap.insert({name, conn});
        std::cout << "客户端：" << name << "加入啦" << std::endl;
        conn.setContext("name", name);
        // request.param("");
    }

    void onRead(const WebsocketConn &conn) override
    {
        auto message = conn.recvMessage();
        if (message.isClosed())
        {
            this->onClose(conn);
            return;
        }
        auto name = conn.getContext("name");
        std::cout << "收到消息来自：" << name << "的消息" << std::endl;
        std::cout << "消息长度：" << message.getLength() << std::endl;
        std::cout << "消息内容：" << message.getData() << std::endl;

        WebsocketMessage sendMessage;
        std::string reply = name + "说： " + std::string(message.getData());
        sendMessage.setData(reply.data(), (int64_t) reply.size());
        this->broadcast(sendMessage, name);
    }

    void onWrite(const WebsocketConn &conn) override
    {
    }

    void onClose(const WebsocketConn &conn) override
    {
        auto name = conn.getContext("name");
        std::cout << "客户端：" << name << "下线！" << std::endl;
        this->connMap.extract(name);
    }

    ~MyHandler() override = default;

private:
    [[nodiscard]] bool checkExist(const std::string &name, const WebsocketConn &conn) const
    {
        auto iter = connMap.find(name);
        if (iter != connMap.end())
        {
            WebsocketMessage sendMessage;
            std::string reply = "name已经被其他客户端占有，请更换name后重新连接";
            sendMessage.setData(reply.data(), (int64_t) reply.size());
            conn.sendMessage(sendMessage);
            conn.close();
            return true;
        }
        return false;
    }

    void broadcast(WebsocketMessage &message, const std::string &sender)
    {
        for (const auto &item: this->connMap)
        {
            if (item.second.getContext("name") == sender) // 不需要给自己发
            {
                continue;
            }
            item.second.sendMessage(message);
        }
    }

private:
    std::map<std::string, const WebsocketConn &> connMap{};
};

void init(TinyWebsocketServer &server)
{
    server.handle("/ws", MyHandler::getInstance());
}