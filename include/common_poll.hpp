//
// Created by Administrator on 2023/12/14.
//

#ifndef TINY_WEBSOCKET_COMMON_POLL_H
#define TINY_WEBSOCKET_COMMON_POLL_H

#include <unistd.h>
#include "global.hpp"
#include "conn_ctx.hpp"

class BaseEventHandler : public EventHandler
{
public:
    explicit BaseEventHandler() = default;

    virtual void eventAdd(int fd) = 0;

public:
    int connEnter();

    void connExit(int fd);

public:
    int poll_fd{};
    int socket_fd{};
    std::map<int, const ConnCtx *> conn_map;
    WebsocketHandler *handler{};
    std::string path;
};

#endif //TINY_WEBSOCKET_COMMON_POLL_H
