//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_CORE_H
#define TINY_WEBSOCKET_CORE_H

#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <functional>
#include "websocket_connect.hpp"
#include "global.hpp"
#include "tcp_net_util.hpp"
#include "event.hpp"
#include "websocket_handler.hpp"

constexpr char *DEFAULT_HOST = (char *) "127.0.0.1";
constexpr int DEFAULT_PORT = 8080;
constexpr bool DEFAULT_BLOCK = true;
constexpr int DEFAULT_BACKLOG = 1024;
constexpr bool DEFAULT_REUSEADDR = true;

class TinyWebsocketServer
{
public:
    explicit TinyWebsocketServer() :
            TinyWebsocketServer(DEFAULT_HOST, DEFAULT_PORT)
    {}

    explicit TinyWebsocketServer(std::string host, int port) :
            TinyWebsocketServer(std::move(host), port, DEFAULT_BLOCK)
    {}

    explicit TinyWebsocketServer(std::string host, int port, bool block)
            : TinyWebsocketServer(std::move(host), port, block, DEFAULT_BACKLOG)
    {}

    explicit TinyWebsocketServer(std::string host, int port, bool block, int backlog)
            : host(std::move(host)), port(port), block(block), backlog(backlog)
    {
    }

    ~TinyWebsocketServer();

public:
    void setBlock(bool block);

    void setHost(const std::string &host);

    void setPort(int port);

    void run();

    void handle(const std::string &path, WebsocketHandler *handler);

    void setReuseaddr(bool reuseaddr);

private:
    void setServerAddr();

    void bind();

    void listen() const;

    void accept();

private:
    std::string host;
    int port;
    bool block;
    int backlog;
    bool reuseaddr;
    int fd;
    struct sockaddr_in server_addr{};
    volatile bool loop{true};
    std::string path{};
    WebsocketHandler *handler;
    EventHandler *eventHandler;
};

#endif //TINY_WEBSOCKET_CORE_H
