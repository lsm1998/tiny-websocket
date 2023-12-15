//
// Created by 刘时明 on 2023/12/15.
//

#include "common_poll.hpp"

void BaseEventHandler::connExit(int fd)
{
    auto iter = conn_map.find(fd);
    if (iter != conn_map.end())
    {
        handler->onClose(*iter->second->getConn());
        delete iter->second;
        conn_map.extract(fd);
    }
}

int BaseEventHandler::connEnter()
{
    struct sockaddr_in client_addr{};
    socklen_t client;
    int client_fd = accept(this->socket_fd, (struct sockaddr *) &client_addr, &client);
    if (client_fd == -1)
    {
        perror("accept error");
        return -1;
    }
    WebsocketConn *conn = new WebsocketConnect(client_fd, client_addr);
    this->conn_map.insert({client_fd, new ConnCtx(conn)});
    // 调用自定义的事件ADD函数
    this->eventAdd(client_fd);
    return client_fd;
}
