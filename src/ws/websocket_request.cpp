//
// Created by Administrator on 2023/12/13.
//

#include "websocket_request.hpp"

WebsocketRequest::WebsocketRequest(int fd) : fd(fd), request(fd)
{
}

bool WebsocketRequest::handshake()
{
    if (!request.isInvalid())
    {
        return false;
    }
    if (request.method() != "GET")
    {
        return false;
    }
    if (request.getHeader("Connection") != "Upgrade")
    {
        return false;
    }
    if (request.getHeader("Upgrade") != "websocket")
    {
        return false;
    }
    if (request.getHeader("Sec-WebSocket-Version") != "13")
    {
        return false;
    }
    this->sec_websocket_key = request.getHeader("Sec-WebSocket-Key");
    if (this->sec_websocket_key.empty())
    {
        return false;
    }
    this->sec_webSocket_extensions = request.getHeader("Sec-WebSocket-Extensions");

    // 握手响应
    HttpResponse response(this->fd);
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
