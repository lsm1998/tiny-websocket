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
    this->loop = false;
    if (this->eventHandler)
    {
        this->eventHandler->destroy();
        delete this->eventHandler;
    }
    delete this->handler;
    close(this->fd);
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
    if ((this->fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("reason");
        throw std::runtime_error("setServerAddr fail");
    }
    memset(&this->server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    inet_aton(this->host.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(this->port);
}

void TinyWebsocketServer::bind()
{
    int optval = 1;
    if (this->reuseaddr && setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("reason");
        throw std::runtime_error("setsockopt fail");
    }

    if (::bind(this->fd, (struct sockaddr *) &this->server_addr, sizeof(this->server_addr)) < 0)
    {
        perror("reason");
        throw std::runtime_error("bind fail");
    }
}

void TinyWebsocketServer::listen() const
{
    // 监听
    if (::listen(this->fd, this->backlog) < 0)
    {
        perror("reason");
        throw std::runtime_error("listen fail");
    }
}

void TinyWebsocketServer::accept()
{
//    if (netSetBlock(this->fd, this->block ? 0 : 1) < 0)
//    {
//        throw std::runtime_error("netSetBlock fail");
//    }
    netSetBlock(this->fd, true);

    this->eventHandler = createEventHandler(this->path , this->handler);
    this->eventHandler->create(this->fd);
    std::cout << "start http server on " << this->host << ":" << this->port << "..." << std::endl;
    while (this->loop)
    {
        this->eventHandler->poll(this->fd);
    }
}

void TinyWebsocketServer::handle(const std::string &path, WebsocketHandler *handler)
{
    this->path = path;
    this->handler = handler;
}

void TinyWebsocketServer::setReuseaddr(bool reuseaddr)
{
    this->reuseaddr = reuseaddr;
}
