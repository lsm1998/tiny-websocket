//
// Created by Administrator on 2023/12/14.
//

#ifndef TINY_WEBSOCKET_COMMON_POLL_H
#define TINY_WEBSOCKET_COMMON_POLL_H

template<typename T>
class BaseEventHandler
{
public:
    explicit BaseEventHandler() = default;

    virtual void eventAdd(int fd) = 0;

public:
    void connEnter()
    {
        struct sockaddr_in client_addr{};
        socklen_t client;
        int client_fd = accept(this->socket_fd, (struct sockaddr *) &client_addr, &client);
        if (client_fd == -1)
        {
            perror("accept error");
            return;
        }
        WebsocketConn *conn = new WebsocketConnect(client_fd, client_addr);
        conn_map.insert(std::pair<int, const ConnCtx *>(client_fd, new ConnCtx(conn)));
        // 调用自定义的事件ADD函数
        this->eventAdd(client_fd);
    }

    void connExit(int fd)
    {
        auto iter = conn_map.find(fd);
        if (iter != conn_map.end())
        {
            handler->onClose(*iter->second->getConn());
            delete iter->second;
            conn_map.extract(fd);
        }
        close(fd);
    }

public:
    int poll_fd{};
    int socket_fd{};
    std::map<int, const ConnCtx *> conn_map{};
    WebsocketHandler *handler{};
    std::string path;
};

#endif //TINY_WEBSOCKET_COMMON_POLL_H
