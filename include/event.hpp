//
// Created by Administrator on 2023/12/13.
//

#ifndef TINY_WEBSOCKET_EVENT_H
#define TINY_WEBSOCKET_EVENT_H

#include "websocket_handler.hpp"

constexpr int DEFAULT_EVENT_SIZE = 1024;

class EventHandler
{
public:
    virtual void create(int fd) = 0;

    virtual void poll(int fd, WebsocketHandler &handler) = 0;

    virtual void destroy() = 0;

    virtual ~EventHandler() = default;
};

extern EventHandler *createEventHandler();

#endif //TINY_WEBSOCKET_EVENT_H
