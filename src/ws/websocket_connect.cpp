//
// Created by Administrator on 2023/12/13.
//

#include "websocket_connect.hpp"

size_t WebsocketConnect::sendMessage(WebsocketConn::MessageType type, char *buf, size_t len)
{
    return write(this->fd, buf, len);
}

size_t WebsocketConnect::recvMessage(WebsocketConn::MessageType type, char *buf, size_t len)
{
    return read(this->fd, buf, len);
}

std::string WebsocketConnect::connId()
{
    return this->conn_id;
}

WebsocketConnect::WebsocketConnect(int fd, struct sockaddr_in client_addr)
{
    this->fd = fd;
    this->client_addr = client_addr;
    // 设置非阻塞
    netSetBlock(this->fd, true);
    // 连接id
    SHA1 sha1;
    sha1 << std::to_string(fd).data();
    sha1.Result(nullptr);
    this->conn_id = sha1.GetDigestString();
}

sockaddr_in WebsocketConnect::remoteAddr()
{
    return this->client_addr;
}