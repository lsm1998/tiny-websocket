//
// Created by 刘时明 on 2023/12/14.
//

#ifndef TINY_WEBSOCKET_CONN_CTX_H
#define TINY_WEBSOCKET_CONN_CTX_H

#include "global.hpp"
#include "websocket_connect.hpp"

class ConnCtx
{
public:
    explicit ConnCtx(WebsocketConn *conn) : conn(conn), is_handshake(false)
    {};

    ~ConnCtx();

    [[nodiscard]] bool isHandshake() const;

    [[nodiscard]] WebsocketConn *getConn() const;

    void handshake() const;

private:
    mutable bool is_handshake;
    WebsocketConn *conn;
};

#endif //TINY_WEBSOCKET_CONN_CTX_H
