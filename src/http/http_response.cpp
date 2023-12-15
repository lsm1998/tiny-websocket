//
// Created by 刘时明 on 2023/12/9.
//

#include "http_response.hpp"

const std::map<int, std::string> CODE_MAP = {
        {101, "Switching Protocols"},
        {200, "OK"},
        {400, "Bad Request"},
        {404, "Not Found"},
        {500, "Internal Server Error"},
};

HttpResponse::HttpResponse(int fd)
{
    this->fd = fd;
}

void HttpResponse::done() const
{
    if (this->_isDone)
    {
        return;
    }
    // 先写状态行
    this->writeRespLine();

    // 先写响应头
    this->writeRespHeader();

    // websocket协议不需要写响应体
    // 写响应体
    this->writeRespBody();
}

void HttpResponse::setHeader(const std::string &key, const std::string &value)
{
    this->header[key] = value;
}

HttpResponse::~HttpResponse()
{
    this->done();
}

void HttpResponse::write(const char *data, size_t len)
{
    this->buf.write(data, (int) len);
}

void HttpResponse::setStatusCode(int code)
{
    this->_code = code;
}

void HttpResponse::writeRespLine() const
{
    auto desc = CODE_MAP.find(this->_code)->second;
    auto code = std::to_string(this->_code);
    ::write(this->fd, VERSION, strlen(VERSION));
    ::write(this->fd, BLANK, strlen(BLANK));
    ::write(this->fd, code.c_str(), code.size());
    ::write(this->fd, BLANK, strlen(BLANK));
    ::write(this->fd, desc.c_str(), desc.size());
    ::write(this->fd, SEPARATOR, 2);
}

void HttpResponse::writeRespHeader() const
{
    for (auto &item: this->header)
    {
        this->writeWithKV(item.first, item.second);
    }
}

void HttpResponse::writeRespBody() const
{
    auto data = this->buf.str();
    this->writeWithKV("Content-Length", std::to_string(data.size()));

    ::write(this->fd, SEPARATOR, 2);

    if (data.empty())
    {
        return;
    }
    ::write(this->fd, data.data(), data.size());
}

void HttpResponse::writeWithKV(const std::string &key, const std::string &value) const
{
    ::write(this->fd, key.data(), key.size());
    ::write(this->fd, COLON, 1);
    ::write(this->fd, BLANK, 1);
    ::write(this->fd, value.data(), value.size());
    ::write(this->fd, SEPARATOR, 2);
}