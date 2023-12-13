//
// Created by 刘时明 on 2023/12/9.
//

#ifndef TINY_HTTP_HTTP_RESPONSE_H
#define TINY_HTTP_HTTP_RESPONSE_H

#include "global.hpp"
#include <map>

extern const std::map<int, std::string> CODE_MAP;

class HttpResponse
{
public:
    explicit HttpResponse(int fd);

    ~HttpResponse();

    HttpResponse(const HttpResponse &response) = delete;

    void setHeader(const std::string &key, const std::string &value);

    void write(const char *data, size_t len);

    void setStatusCode(int code);

private:
    void writeWithKV(const std::string &key, const std::string &value) const;

    void writeRespLine() const;

    void writeRespHeader() const;

    void writeRespBody() const;

    void done() const;

private:
    int fd{};

    int _code{200};

    bool _isDone{false};

    std::stringstream buf{};

    std::map<std::string, std::string> header{};
};

#endif //TINY_HTTP_HTTP_RESPONSE_H
