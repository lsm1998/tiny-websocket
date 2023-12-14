//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
#define TINY_WEBSOCKET_WEBSOCKET_CONNECT_H

#include "event.hpp"
#include "global.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "sha1.hpp"
#include "base64.hpp"
#include <netinet/in.h>
#include <unistd.h>

class WebsocketConn
{
public:
    enum MessageType
    {
        TEXT = 1,
        BINARY = 2,
        CLOSE = 8,
        PING = 9,
        PONG = 10
    };

public:
    virtual size_t sendMessage(MessageType type, char *buf, size_t len) = 0;

    virtual size_t recvMessage(MessageType type, char *buf, size_t len) = 0;

    virtual ~WebsocketConn() = default;

    virtual std::string connId() = 0;

    virtual sockaddr_in remoteAddr() = 0;
};

class WebsocketConnect : public WebsocketConn
{
public:
    explicit WebsocketConnect(int socket_fd,struct sockaddr_in client_addr);

    ~WebsocketConnect() override = default;

    size_t sendMessage(MessageType type, char *buf, size_t len) override;

    size_t recvMessage(MessageType type, char *buf, size_t len) override;

    std::string connId() override;

    sockaddr_in remoteAddr() override;

private:
    int fd;

    std::string conn_id;

    struct sockaddr_in client_addr{};
};

#endif //TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
