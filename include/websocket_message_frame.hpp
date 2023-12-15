//
// Created by Administrator on 2023/12/15.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_MESSAGE_FRAME_H
#define TINY_WEBSOCKET_WEBSOCKET_MESSAGE_FRAME_H

#include "global.hpp"
#include "websocket_message.hpp"

class WebsocketMessageFrame
{
public:
    explicit WebsocketMessageFrame(WebsocketMessage *message);

    ~WebsocketMessageFrame();

    void readHeader();

    void readPayload();

    void writeHeader(int i) const;

    void writePayload(int i) const;

    [[nodiscard]] bool invalid() const;

    [[nodiscard]] uint8_t fin() const;

    [[nodiscard]] uint8_t opCode() const;

    [[nodiscard]] uint8_t mask() const;

    [[nodiscard]] int64_t length() const;

private:
    /**
     * 将8字节的char数组转换为int64_t
     * @param buf
     * @return
     */
    static int64_t char_2_int64(const char *buf);

private:
    // 第一个字节
    uint8_t byte1_{};
    // 第二个字节
    uint8_t byte2_{};
    // 掩码
    uint8_t masking_key_[4]{};
    // 负载长度
    int64_t payload_length{};
    // 是否是错误消息
    bool is_invalid{};
    // 文件描述符
    // int fd{};
    // 消息结构
    WebsocketMessage *message;
};

#endif //TINY_WEBSOCKET_WEBSOCKET_MESSAGE_FRAME_H
