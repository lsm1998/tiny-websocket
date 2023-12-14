//
// Created by Administrator on 2023/12/13.
//

#include "websocket_connect.hpp"

size_t WebsocketConnect::sendMessage(WebsocketConn::MessageType type, char *buf, size_t len)
{
    return 0;
}

size_t WebsocketConnect::recvMessage(WebsocketConn::MessageType type, char *buf, size_t len)
{
    return 0;
}

std::string WebsocketConnect::connId()
{
    return this->conn_id;
}

WebsocketConnect::WebsocketConnect(int fd, struct sockaddr_in client_addr)
{
    this->fd = fd;
    this->client_addr = client_addr;
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

bool WebsocketConnect::isHandshake()
{
    return this->is_handshake;
}
