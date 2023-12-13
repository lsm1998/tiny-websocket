//
// Created by 刘时明 on 2023/12/9.
//

#include "http_request.hpp"

HttpRequest::HttpRequest(int fd)
{
    this->fd = fd;

    // 解析请求行
    this->parseRequestLine();

    // 解析请求头
    this->parseRequestHeader();

    // 是否有数据可读
    if (this->contentLength() > 0)
    {
        this->bodyReady = true;
    }
}

HttpRequest::String HttpRequest::path() const
{
    return this->_path;
}

void HttpRequest::parseRequestLine()
{
    char buf[MAX_BUF_SIZE];

    size_t len = readUntilDelimiter(this->fd, '\n', buf, MAX_BUF_SIZE);
    if (len <= 0)
    {
        this->_invalid = true;
        return;
    }
    // 删除\r\n
    buf[len - 2] = '\0';
    auto list = split(buf, ' ');
    if (list.size() != 3)
    {
        this->_invalid = true;
        return;
    }

    // 请求方法
    this->_method = list[0];
    // 请求路径
    {
        auto innerList = split(list[1], '?', 2);
        this->_path = innerList[0];
        if (innerList.size() == 2)
        {
            // 解析query参数
            this->queryRow = innerList[1];
            this->parseQuery();
        }
    }

    {
        auto innerList = split(list[2], '/');
        if (innerList.size() != 2)
        {
            this->_invalid = true;
            return;
        }
        // 协议
        this->_protocol = innerList[0];
        // 版本
        this->_version = innerList[1];
    }
}

bool HttpRequest::isInvalid() const
{
    return this->_invalid;
}

void HttpRequest::parseRequestHeader()
{
    if (this->isInvalid())
    {
        return;
    }

    char buf[MAX_BUF_SIZE];
    while (true)
    {
        memset(buf, 0, MAX_BUF_SIZE);
        size_t len = readUntilDelimiter(this->fd, '\n', buf, MAX_BUF_SIZE);
        if (len <= 0)
        {
            this->_invalid = true;
            break;
        } else if (len == 2 && strcmp(buf, "\r\n") == 0)
        {
            // 读到空行了
            break;
        }
        // 删除\r\n
        buf[len - 2] = '\0';
        auto list = split(buf, ": ", 2);
        if (list.size() != 2)
        {
            this->_invalid = true;
            break;
        }
        auto item = this->_header.find(list[0]);
        if (item == this->_header.end())
        {
            this->_header[list[0]] = {list[1]};
        } else
        {
            item->second.push_back(list[1]);
        }
    }
}

HttpRequest::String HttpRequest::method() const
{
    return this->_method;
}

HttpRequest::String HttpRequest::protocol() const
{
    return this->_protocol;
}

HttpRequest::String HttpRequest::version() const
{
    return this->_version;
}

std::vector<HttpRequest::String> HttpRequest::header(const HttpRequest::String &key) const
{
    auto item = this->_header.find(key);
    return item->second;
}

HttpRequest::HttpHeader HttpRequest::headers() const
{
    return this->_header;
}

void HttpRequest::parseQuery()
{
    if (this->queryRow.empty())
    {
        return;
    }
    auto list = split(this->queryRow, '&');
    for (auto &str: list)
    {
        auto innerList = split(str, '=', 2);
        if (innerList.size() != 2)
        {
            this->_invalid = true;
            return;
        }
        auto item = this->_query.find(innerList[0]);
        if (item == this->_query.end())
        {
            this->_query[innerList[0]] = {innerList[1]};
        } else
        {
            item->second.push_back(innerList[1]);
        }
    }
}

std::vector<HttpRequest::String> HttpRequest::query(const HttpRequest::String &key) const
{
    auto item = this->_query.find(key);
    return item->second;
}

HttpRequest::FormData HttpRequest::queryMap() const
{
    return this->_query;
}

HttpRequest::String HttpRequest::param(const HttpRequest::String &name) const
{
    return this->params.find(name)->second;
}

void HttpRequest::setParam(const HttpRequest::String &name, const HttpRequest::String &value)
{
    this->params[name] = value;
}

size_t HttpRequest::readBody(char *buf, size_t len) const
{
    if (!bodyReady)
    {
        return EOF;
    }
    bodyReady = false; // 不允许读多次
    return ::read(this->fd, buf, len);
}

HttpRequest::String HttpRequest::contentType() const
{
    auto item = this->_header.find("Content-Type");
    if (item == this->_header.end() || item->second.at(0).empty())
    {
        return "";
    }
    return item->second.at(0);
}

int HttpRequest::contentLength() const
{
    auto item = this->_header.find("Content-Length");
    if (item == this->_header.end() || item->second.at(0).empty())
    {
        return 0;
    }
    return std::stoi(item->second.at(0));
}

HttpRequest::String HttpRequest::getHeader(const HttpRequest::String &key) const
{
    auto item = this->_header.find(key);
    if (item == this->_header.end() || item->second.at(0).empty())
    {
        return "";
    }
    return item->second.at(0);
}
