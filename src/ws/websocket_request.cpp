//
// Created by Administrator on 2023/12/13.
//

#include "websocket_request.hpp"

WebsocketRequest::WebsocketRequest(int fd, const std::string &path) : fd(fd), request(fd)
{
    this->is_invalid = !this->handshake(path);
}

bool WebsocketRequest::handshake(const std::string &path)
{
    HttpResponse response(this->fd);
    if (request.isInvalid() || request.getHeader("Connection") != "Upgrade" ||
        request.getHeader("Upgrade") != "websocket" || request.getHeader("Sec-WebSocket-Version") != "13")
    {
        return this->fail(400, response);
    }
    if (request.method() != "GET" || request.path() != path)
    {
        return this->fail(404, response);
    }
    this->sec_websocket_key = request.getHeader("Sec-WebSocket-Key");
    if (this->sec_websocket_key.empty())
    {
        return false;
    }
    this->sec_webSocket_extensions = request.getHeader("Sec-WebSocket-Extensions");

    // 握手响应
    response.setStatusCode(101);
    response.setHeader("Upgrade", "websocket");
    response.setHeader("Connection", "Upgrade");

    SHA1 sha1;
    sha1 << this->sec_websocket_key.data();
    sha1 << WEBSOCKET_GUID;
    sha1.Result(nullptr);
    response.setHeader("Sec-WebSocket-Accept", base64_encode(sha1.GetDigestString()));
    return true;
}

bool WebsocketRequest::fail(int code, HttpResponse &response)
{
    response.setStatusCode(code);
    return false;
}

bool WebsocketRequest::isInvalid() const
{
    return this->is_invalid;
}

std::vector<std::string> WebsocketRequest::getQueryArray(const std::string &key) const
{
    return this->request.query(key);
}

std::string WebsocketRequest::getQuery(const std::string &key) const
{
    auto list = this->request.query(key);
    if (list.empty())
    {
        return "";
    }
    return list.at(0);
}
