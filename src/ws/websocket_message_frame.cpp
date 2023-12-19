//
// Created by Administrator on 2023/12/15.
//

#include "websocket_message_frame.hpp"

int64_t WebsocketMessageFrame::char_2_int64(const char *buf)
{
    int64_t value = 0;
    for (int8_t i = 7; i >= 0; i--)
    {
        value <<= 8;
        value |= buf[i];
    }
    return value;
}

bool WebsocketMessageFrame::invalid() const
{
    return this->is_invalid;
}

uint8_t WebsocketMessageFrame::fin() const
{
    return byte1_ >> 7;
}

uint8_t WebsocketMessageFrame::opCode() const
{
    return this->byte1_ & 0x0f;
}

uint8_t WebsocketMessageFrame::mask() const
{
    return this->byte2_ >> 7;
}

int64_t WebsocketMessageFrame::length() const
{
    return payload_length;
}

WebsocketMessageFrame::~WebsocketMessageFrame()
{
}

WebsocketMessageFrame::WebsocketMessageFrame(WebsocketMessage *message)
{
    this->message = message;
}

void WebsocketMessageFrame::readHeader()
{
    char buf[8];
    if (read(this->message->fd, buf, 2) != 2) // 读取2个字节
    {
        this->is_invalid = true;
        return;
    }

    {
        // 解析第一个字节
        this->byte1_ = buf[0];
        this->message->messageType = static_cast<MessageType>(this->opCode());
    }

    {
        // 解析第二个字节
        this->byte2_ = buf[1];

        int8_t len = byte2_ & 0b01111111;
        if (len <= 125)
        {
            this->payload_length = (int64_t) len;
        } else if (len == 126)
        {
            if (read(this->message->fd, buf, 2) != 2) // 读取2个字节
            {
                this->is_invalid = true;
                return;
            }
            this->payload_length = char_2_int64(buf);
        } else if (len == 127)
        {
            if (read(this->message->fd, buf, 8) != 8) // 读取8个字节
            {
                this->is_invalid = true;
                return;
            }
            this->payload_length = char_2_int64(buf);
        }

        if (this->mask() == 1)
        {
            if (read(this->message->fd, this->masking_key_, 4) != 4) // 读取4个字节
            {
                this->is_invalid = true;
                return;
            }
        }
    }
}

void WebsocketMessageFrame::readPayload()
{
    this->message->data = new char[this->payload_length + 1];

    size_t read_len = read(this->message->fd, this->message->data, this->length() + 1);
    if (read_len != this->length())
    {
        // 读到的数据长度不对
        this->is_invalid = true;
        std::cout << "读到的数据长度不对，期望长度：" << this->length() << "，实际长度：" << read_len << "，数据："
                  << this->message->data << std::endl;
        return;
    }
    if (this->mask() == 1)
    {
        for (int i = 0; i < this->length(); i++)
        {
            this->message->data[i] ^= this->masking_key_[i % 4];
        }
    }
    this->is_invalid = false;
}

void WebsocketMessageFrame::writeHeader(int fd) const
{
    // 写入第一个字节
    uint8_t buf[1];
    uint8_t fin = 1;
    buf[0] = fin << 7 | this->message->messageType;
    write(fd, buf, 1);

    // 写入第二个字节
    uint8_t mask = 0;
    buf[0] = mask << 7;
    // 写入长度
    uint64_t length = this->message->getLength();
    if (length <= 125)
    {
        buf[0] |= length;
        write(fd, buf, 1);
    } else if (this->length() <= 65535)
    {
        buf[0] |= 126;
        write(fd, buf, 1);
        buf[0] = length >> 8;
        write(fd, buf, 1);
        buf[0] = length & 0xff;
        write(fd, buf, 1);
    } else
    {
        buf[0] |= 127;
        write(fd, buf, 1);
        for (int i = 7; i >= 0; i--)
        {
            buf[0] = length >> (i * 8) & 0xff;
            write(fd, buf, 1);
        }
    }
}

void WebsocketMessageFrame::writePayload(int fd) const
{
    // 写入body
    write(fd, this->message->data, this->message->getLength());
}

