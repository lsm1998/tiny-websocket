//
// Created by 刘时明 on 2023/12/14.
//

#include "conn_ctx.hpp"

ConnCtx::~ConnCtx()
{
    delete this->conn;
}

bool ConnCtx::isHandshake() const
{
    return this->is_handshake;
}

WebsocketConn *ConnCtx::getConn() const
{
    return this->conn;
}

void ConnCtx::handshake() const
{
    this->is_handshake = true;
}
