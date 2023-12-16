//
// Created by Administrator on 2023/12/13.
//

#include "global.hpp"
#include "core.hpp"
#include "websocket_request.hpp"
#include "conn_ctx.hpp"
#include "common_poll.hpp"

#ifdef MACOS

#include <sys/event.h>

constexpr int kReadEvent = 1;
constexpr int kWriteEvent = 2;

class KqueueEventHandler : public BaseEventHandler
{
public:
    explicit KqueueEventHandler(const std::string &path, WebsocketHandler *handler)
    {
        this->path = path;
        this->handler = handler;
    }

    int create(int fd) override
    {
        this->socket_fd = fd;
        if ((this->poll_fd = kqueue()) == -1)
        {
            perror("kqueue");
            exit(EXIT_FAILURE);
        }
        return updateEvents(this->socket_fd, kReadEvent, false);
    }

    void poll(int fd) override
    {
        struct kevent events[DEFAULT_EVENT_SIZE]{};
        memset(events, 0, sizeof(events));
        int n = kevent(this->poll_fd, nullptr, 0, events, DEFAULT_EVENT_SIZE, nullptr);
        std::cout << "poll一次" << std::endl;
        for (int i = 0; i < n; ++i)
        {
            int client_fd = (int) (intptr_t) events[i].ident;
            // 是否是连接退出
            if (events[i].flags & EV_EOF) // 连接退出
            {
                std::cout << "连接退出" << std::endl;
                this->onExit(client_fd);
                continue;
            }
            if (client_fd == this->socket_fd) // 连接进入
            {
                std::cout << "连接进入" << std::endl;
                this->connEnter();
                continue;
            }
            int ev = events[i].filter;

            if (ev == EVFILT_READ) // 读事件
            {
                std::cout << "读事件" << std::endl;
                this->onRead(client_fd);
            }
            if (ev == EVFILT_WRITE && events[i].flags & EV_CLEAR) // 写事件
            {
                std::cout << "写事件" << std::endl;
                this->onWrite(client_fd);
            }
        }
    }

    void destroy() override
    {

    }

private:
    int updateEvents(int fd, int events, bool modify)
    {
        struct kevent ev[2];
        int n = 0;
        if (events & kReadEvent)
        {
            EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0,
                   (void *) (intptr_t) fd);
        } else if (modify)
        {
            EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void *) (intptr_t) fd);
        }
        if (events & kWriteEvent)
        {
            EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0,
                   (void *) (intptr_t) fd);
        } else if (modify)
        {
            EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void *) (intptr_t) fd);
        }
        return kevent(this->poll_fd, ev, n, nullptr, 0, nullptr);
    }

    void onExit(int client_fd)
    {
        this->connExit(client_fd);
    }

    void onRead(int client_fd)
    {
        auto item = this->conn_map.find(client_fd);
        if (item == this->conn_map.end())
        {
            return;
        }
        if (item->second->isHandshake())
        {
            handler->onRead(*item->second->getConn());
        } else
        {
            WebsocketRequest request(client_fd, this->path);
            if (request.isInvalid())
            {
                std::cout << "握手失败" << std::endl;
                onExit(client_fd);
                return;
            }
            item->second->handshake();
            std::cout << "握手完成" << std::endl;
            handler->onAccept(request, *item->second->getConn());
        }
    }

    void onWrite(int client_fd)
    {
        updateEvents(client_fd, kReadEvent, true);
    }

public:
    void eventAdd(int fd) override
    {
        this->updateEvents(fd, kReadEvent | kWriteEvent, false);
    }
};

EventHandler *createEventHandler(const std::string &path, WebsocketHandler *handler)
{
    return new KqueueEventHandler(path, handler);
}


#endif