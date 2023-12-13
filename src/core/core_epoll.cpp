//
// Created by Administrator on 2023/12/13.
//

#include "global.hpp"
#include "core.hpp"

#ifdef LINUX

#include "websocket_connect.hpp"
#include <sys/epoll.h>

class EpollEventHandler : public EventHandler
{
public:
    void create(int fd) override
    {
        this->socket_fd = fd;
        this->epoll_fd = epoll_create(1);
        epoll_event event{};
        event.events = EPOLLIN;
        event.data.fd = this->socket_fd;
        event.events |= EPOLLET; // 设置边缘触发
        epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &event);
    }

    void poll(int fd, WebsocketHandler &handler) override
    {
        memset(this->events, 0, sizeof(this->events));
        int n = epoll_wait(this->epoll_fd, events, DEFAULT_EVENT_SIZE, -1);
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
                this->connExit(&events[i], handler);
                continue;
            }
            if (events[i].data.fd == this->socket_fd)  // 连接加入
            {
                struct sockaddr_in client_addr{};
                socklen_t client;
                int client_fd = accept(this->socket_fd, (struct sockaddr *) &client_addr, &client);
                if (client_fd == -1)
                {
                    perror("accept error");
                    continue;
                }
                connEnter(client_fd, handler);
                continue;
            }
            if (events[i].events & EPOLLIN) // 是否可读
            {
                auto iter = conn_map.find(events[i].data.fd);
                if (iter != conn_map.end())
                {
                    handler.onRead(*iter->second);
                }
            }
            if (events[i].events & EPOLLOUT) // 是否可写
            {
                auto iter = conn_map.find(events[i].data.fd);
                if (iter != conn_map.end())
                {
                    handler.onWrite(*iter->second);
                }
            }
        }
    }

    void destroy() override
    {

    }

    ~EpollEventHandler() override
    {
        close(this->epoll_fd);
    }

private:
    void connEnter(int fd, WebsocketHandler &handler)
    {
        struct epoll_event ev{};
        netSetBlock(fd, true);
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &ev);
        WebsocketConn *conn = new WebsocketConnect(fd);
        conn_map.insert(std::pair<int, WebsocketConn *>(fd, conn));
        handler.onAccept(*conn);
    }

    void connExit(struct epoll_event *event, WebsocketHandler &handler)
    {
        int fd = event->data.fd;
        auto iter = conn_map.find(fd);
        if (iter != conn_map.end())
        {
            handler.onClose(*iter->second);
            delete iter->second;
            conn_map.erase(iter);
        }
        epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, event);
        close(fd);
    }

private:
    int epoll_fd{};

    int socket_fd{};

    epoll_event events[DEFAULT_EVENT_SIZE]{};

    std::map<int, WebsocketConn *> conn_map{};
};

EventHandler *createEventHandler()
{
    return new EpollEventHandler();
}

#endif