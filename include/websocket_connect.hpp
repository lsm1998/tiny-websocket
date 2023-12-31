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

enum MessageType
{
    TEXT = 1,
    BINARY = 2,
    CLOSE = 8,
    PING = 9,
    PONG = 10
};

class WebsocketConn
{
public:
    virtual size_t sendMessage(WebsocketMessage &message) const = 0;

    virtual WebsocketMessage &&recvMessage() const = 0;

    virtual ~WebsocketConn() = default;

    virtual std::string connId() const = 0;

    virtual sockaddr_in remoteAddr() const = 0;

    virtual void setContext(const std::string &key, const std::string &value) const;

    virtual std::string getContext(const std::string &key) const;

    virtual void close() const = 0;

private:
    mutable std::map<std::string, std::string> context_map{};
};

class WebsocketConnect : public WebsocketConn
{
public:
    explicit WebsocketConnect(int socket_fd, struct sockaddr_in client_addr);

    ~WebsocketConnect() override;

    size_t sendMessage(WebsocketMessage &message) const override;

    WebsocketMessage &&recvMessage() const override;

    std::string connId() const override;

    sockaddr_in remoteAddr() const override;

    void close() const override;

private:
    int fd;

    std::string conn_id;

    struct sockaddr_in client_addr{};
};

#endif //TINY_WEBSOCKET_WEBSOCKET_CONNECT_H
