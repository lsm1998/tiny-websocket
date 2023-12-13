//
// Created by Administrator on 2023/12/13.
//

#include "global.hpp"
#include "core.hpp"

#ifdef MACOS

#include <sys/event.h>

constexpr int kReadEvent = 1;
constexpr int kWriteEvent = 2;

class KqueueEventHandler : public EventHandler
{
public:
    void create(int fd) override
    {
        this->socket_fd = fd;
        if ((this->kq_fd = kqueue()) == -1)
        {
            perror("kqueue");
            exit(EXIT_FAILURE);
        }
        updateEvents(kReadEvent, false);
    }

    void poll(int fd, WebsocketHandler &handler) override
    {
        memset(this->events, 0, sizeof(this->events));
        struct timespec timeout{};
        timeout.tv_sec = -1;
        timeout.tv_nsec = -1;
        int n = kevent(this->kq_fd, nullptr, 0, this->events, DEFAULT_EVENT_SIZE, &timeout);
        for (int i = 0; i < n; ++i)
        {
            int client_fd = (int) (intptr_t) events[i].udata;
            // 是否是连接退出
            if (events[i].flags & EV_EOF)
            {
                onExit(client_fd);
                continue;
            }
            int ev = events[i].filter;
            if (ev == EVFILT_READ) // 读事件
            {
                if (client_fd == this->socket_fd)
                {
                    handleAccept(client_fd);
                } else
                {
                    handleRead(client_fd);
                }
            }
            if (ev == EVFILT_WRITE && events[i].flags & EV_CLEAR) // 写事件
            {
                handleWrite(client_fd);
            }
        }
    }

    void destroy() override
    {

    }

private:
    int updateEvents(int events, bool modify)
    {
        struct kevent ev[2];
        int n = 0;
        if (events & kReadEvent)
        {
            EV_SET(&ev[n++], this->socket_fd, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0,
                   (void *) (intptr_t) this->socket_fd);
        } else if (modify)
        {
            EV_SET(&ev[n++], this->socket_fd, EVFILT_READ, EV_DELETE, 0, 0, (void *) (intptr_t) this->socket_fd);
        }
        if (events & kWriteEvent)
        {
            EV_SET(&ev[n++], this->socket_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0,
                   (void *) (intptr_t) this->socket_fd);
        } else if (modify)
        {
            EV_SET(&ev[n++], this->socket_fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void *) (intptr_t) this->socket_fd);
        }
        printf("%s fd %d events read %d write %d\n", modify ? "mod" : "add", this->socket_fd, events & kReadEvent,
               events & kWriteEvent);
        return kevent(this->kq_fd, ev, n, nullptr, 0, nullptr);
    }

    void onExit(int client_fd)
    {}

    void handleAccept(int client_fd)
    {}

    void handleRead(int client_fd)
    {}

    void handleWrite(int client_fd)
    {}

private:
    int socket_fd{};
    int kq_fd{};
    struct kevent events[DEFAULT_EVENT_SIZE]{};
};

EventHandler *createEventHandler()
{
    return new KqueueEventHandler();
}


#endif