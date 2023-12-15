//
// Created by Administrator on 2023/12/13.
//

#include "websocket_message.hpp"
#include "websocket_message_frame.hpp"

WebsocketMessage::WebsocketMessage(int fd) : fd(fd)
{
    auto frame = WebsocketMessageFrame(this);
    frame.readHeader();
    if (frame.invalid())
    {
        this->closed = true;
        return;
    }
    this->length = frame.length();
    this->data = new char[this->length];
    frame.readPayload();
    if (frame.invalid())
    {
        this->closed = true;
        return;
    }
    if (frame.fin() == 0)
    {
        // todo 处理分片消息
    }
}

WebsocketMessage::~WebsocketMessage()
{
    if (this->fd > 0) // 代表是读取的消息，需要释放内存
    {
        delete[]this->data;
        std::cout << "读消息的message回收掉了" << std::endl;
    } else
    {
        // 代表是写入的消息，不需要释放内存
        std::cout << "写消息的message回收掉了" << std::endl;
    }
}

char *WebsocketMessage::getData() const
{
    return this->data;
}

int64_t WebsocketMessage::getLength() const
{
    return this->length;
}

bool WebsocketMessage::isClosed() const
{
    return this->closed || this->messageType == MessageType::CLOSE;
}

void WebsocketMessage::setData(char *buf, int64_t length)
{
    this->data = buf;
    this->length = length;
}

WebsocketMessage &&WebsocketMessageWapper::read(int fd)
{
    auto *message = new WebsocketMessage(fd);
    return std::move(*message);
}

ssize_t WebsocketMessageWapper::write(int fd, WebsocketMessage &message)
{
    // 发送协议头
    auto frame = WebsocketMessageFrame(&message);
    frame.writeHeader(fd);
    frame.writePayload(fd);
    return 1;
}
