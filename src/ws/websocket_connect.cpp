//
// Created by Administrator on 2023/12/13.
//

#include "websocket_connect.hpp"

size_t WebsocketConnect::sendMessage(WebsocketConn::MessageType type, char *buf, size_t len) const
{
    return write(this->fd, buf, len);
}

size_t WebsocketConnect::recvMessage(WebsocketConn::MessageType type, char *buf, size_t len) const
{
    return read(this->fd, buf, len);
}

std::string WebsocketConnect::connId() const
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

sockaddr_in WebsocketConnect::remoteAddr() const
{
    return this->client_addr;
}

void WebsocketConn::setContext(const std::string &key, const std::string &value) const
{
    this->context_map.insert({key, value});
}

std::string WebsocketConn::getContext(const std::string &key) const
{
    return context_map.at(key);
}
