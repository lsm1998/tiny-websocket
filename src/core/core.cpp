//
// Created by Administrator on 2023/12/13.
//

#include "core.hpp"

void TinyWebsocketServer::run()
{
    // socket
    this->setServerAddr();

    // bind
    this->bind();

    // listen
    this->listen();

    // accept
    this->accept();
}

TinyWebsocketServer::~TinyWebsocketServer()
{

}

void TinyWebsocketServer::setBlock(bool block)
{
    this->block = block;
}

void TinyWebsocketServer::setHost(const std::string &host)
{
    this->host = host;
}

void TinyWebsocketServer::setPort(int port)
{
    this->port = port;
}

void TinyWebsocketServer::setServerAddr()
{

}

void TinyWebsocketServer::bind()
{

}

void TinyWebsocketServer::listen() const
{

}

void TinyWebsocketServer::accept() const
{

}

void TinyWebsocketServer::handle(const std::string &path, const std::function<void(WebsocketConn &)>& function)
{
    this->path = path;
    this->function = function;
}
