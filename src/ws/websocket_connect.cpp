//
// Created by Administrator on 2023/12/13.
//

#include "websocket_connect.hpp"
#include "websocket_message_frame.hpp"

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

WebsocketConnect::~WebsocketConnect()
{
    this->close();
}

void WebsocketConnect::close() const
{
    ::close(this->fd);
}

size_t WebsocketConnect::sendMessage(WebsocketMessage &message) const
{
    return WebsocketMessageWapper::write(this->fd, message);
}

WebsocketMessage&& WebsocketConnect::recvMessage() const
{
    return WebsocketMessageWapper::read(this->fd);
}

void WebsocketConn::setContext(const std::string &key, const std::string &value) const
{
    this->context_map.insert({key, value});
}

std::string WebsocketConn::getContext(const std::string &key) const
{
    auto iter = this->context_map.find(key);
    if (iter == this->context_map.end())
    {
        return "";
    }
    return iter->second;
}
