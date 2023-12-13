//
// Created by 刘时明 on 2023/12/9.
//

#ifndef TINY_HTTP_HTTP_REQUEST_H
#define TINY_HTTP_HTTP_REQUEST_H

#include "global.hpp"
#include "tcp_net_util.hpp"
#include "string_util.hpp"

class HttpRequest
{
    using String = std::string;

    using HttpHeader = std::map<String, std::vector<String>>;

    using FormData = HttpHeader;

public:
    explicit HttpRequest(int fd);

    ~HttpRequest() = default;

    HttpRequest(const HttpRequest &request) = delete;

    /**
     * 暴露给路由设置Param参数的函数
     *
     * @param name
     * @param value
     */
    void setParam(const String &name, const String &value);

    [[nodiscard]] String path() const;

    [[nodiscard]] String method() const;

    [[nodiscard]] bool isInvalid() const;

    [[nodiscard]] String protocol() const;

    [[nodiscard]] String version() const;

    String getHeader(const String &key) const;

    [[nodiscard]] std::vector<String> header(const String &key) const;

    [[nodiscard]] HttpHeader headers() const;

    [[nodiscard]] std::vector<String> query(const String &key) const;

    [[nodiscard]] FormData queryMap() const;

    [[nodiscard]] String param(const String &name) const;

    size_t readBody(char *buf, size_t len) const;

    [[nodiscard]] int contentLength() const;

    [[nodiscard]] String contentType() const;

private:
    void parseRequestLine();

    void parseRequestHeader();

    void parseQuery();

private:
    // 请求是否有效
    bool _invalid{};

    // 请求体是否可读
    mutable bool bodyReady{};

    // 请求方法
    String _method{};

    // 请求路径
    String _path{};

    // 协议
    String _protocol{};

    // 版本
    String _version{};

    // 请求头
    HttpHeader _header{};

    // query参数
    FormData _query{};

    // form参数
    FormData form{};

    // param参数
    std::map<String, String> params;

    // 文件描述符
    int fd{};

    // query row数据
    String queryRow;
};

#endif //TINY_HTTP_HTTP_REQUEST_H
