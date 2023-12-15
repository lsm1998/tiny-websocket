//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_MESSAGE_H
#define TINY_WEBSOCKET_WEBSOCKET_MESSAGE_H

#include "websocket_connect.hpp"

class WebsocketMessage
{
    friend class WebsocketMessageFrame;

public:
    explicit WebsocketMessage(int fd);

    WebsocketMessage() = default;

    ~WebsocketMessage();

    [[nodiscard]] char *getData() const;

    void setData(char *buf, int64_t length);

    [[nodiscard]] int64_t getLength() const;

    [[nodiscard]] bool isClosed() const;

public:
    MessageType messageType;

private:
    int fd{};

    int64_t length{};

    char *data{};

    bool closed = false;
};

class WebsocketMessageWapper
{
public:
    static WebsocketMessage &&read(int fd);

    static ssize_t write(int fd, WebsocketMessage &message);
};

#endif //TINY_WEBSOCKET_WEBSOCKET_MESSAGE_H
