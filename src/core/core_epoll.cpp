//
// Created by Administrator on 2023/12/13.
//

#include "global.hpp"
#include "core.hpp"
#include "websocket_connect.hpp"
#include "conn_ctx.hpp"
#include "common_poll.hpp"

#ifdef LINUX

#include <sys/epoll.h>

#include <utility>

class EpollEventHandler : public EventHandler, public BaseEventHandler<EpollEventHandler>
{
public:
    EpollEventHandler(std::string path, WebsocketHandler *handler)
    {
        this->path = std::move(path);
        this->handler = handler;
    }

    void create(int fd) override
    {
        this->socket_fd = fd;
        this->poll_fd = epoll_create(1);
        epoll_event event{};
        event.events = EPOLLIN;
        event.data.fd = this->socket_fd;
        event.events |= EPOLLET; // 设置边缘触发
        epoll_ctl(this->poll_fd, EPOLL_CTL_ADD, fd, &event);
    }

    void poll(int fd) override
    {
        epoll_event events[DEFAULT_EVENT_SIZE]{};
        memset(events, 0, sizeof(events));
        int n = epoll_wait(this->poll_fd, events, DEFAULT_EVENT_SIZE, -1);
        if (n == -1)
        {
            perror("epoll_wait error");
            return;
        }
        for (int i = 0; i < n; ++i)
        {
            // 连接退出
            if ((events[i].events & EPOLLHUP) || (events[i].events & EPOLLERR) || (!(events[i].events & EPOLLIN)))
            {
                this->onExit(&events[i]);
                continue;
            }
            if (events[i].data.fd == this->socket_fd)  // 连接加入
            {
                this->connEnter();
            }
            if (events[i].events & EPOLLIN) // 是否可读
            {
                this->onRead(&events[i]);
            }
            if (events[i].events & EPOLLOUT) // 是否可写
            {

                this->onWrite(events[i].data.fd);
            }
        }
    }

    void destroy() override
    {

    }

    ~EpollEventHandler() override
    {
        close(this->poll_fd);
    }

private:
    void onExit(struct epoll_event *event)
    {
        this->connExit(event->data.fd);
        epoll_ctl(this->poll_fd, EPOLL_CTL_DEL, event->data.fd, event);
    }

    void onRead(struct epoll_event *event)
    {
        int fd = event->data.fd;
        auto item = this->conn_map.find(fd);
        if (item == this->conn_map.end())
        {
            return;
        }
        if (item->second->isHandshake())
        {
            handler->onRead(*item->second->getConn());
        } else
        {
            WebsocketRequest request(fd);
            if (request.handshake(this->path))
            {
                std::cout << "握手失败" << std::endl;
                onExit(event);
                return;
            }
            handler->onAccept(request, *item->second->getConn());
            item->second->handshake();
        }
    }

    void onWrite(int fd)
    {

    }

public:
    void eventAdd(int fd) override
    {
        struct epoll_event ev{};
        netSetBlock(fd, true);
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(this->poll_fd, EPOLL_CTL_ADD, fd, &ev);
    }
};

EventHandler *createEventHandler(const std::string &path, WebsocketHandler *handler)
{
    return new EpollEventHandler(path, handler);
}

#endif