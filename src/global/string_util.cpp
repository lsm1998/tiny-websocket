//
// Created by Administrator on 2023/12/10.
//

#include "string_util.hpp"

std::vector<std::string> split(const std::string &str, const std::string &delimiter, int n)
{
    std::vector<std::string> tokens;

    size_t pos = 0;
    size_t found;
    int count = 1;

    while ((found = str.find(delimiter, pos)) != std::string::npos && (n == -1 || count++ < n))
    {
        tokens.push_back(str.substr(pos, found - pos));
        pos = found + delimiter.size();
    }

    tokens.push_back(str.substr(pos));

    return tokens;
}

std::vector<std::string> split(const std::string &str, char token, int n)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    int count = 0;

    std::string tokenizedPart;
    while (getline(iss, tokenizedPart, token) && (n == -1 || count++ < n))
    {
        tokens.push_back(tokenizedPart);
    }
    return tokens;
}

template<>
std::string toString(const std::vector<std::string> &val)
{
    std::stringstream buf;
    buf.write("[", 1);
    for (auto &item: val)
    {
        buf.write(item.c_str(), static_cast<std::streamsize>(item.size()));
        buf.write(",", 1);
    }
    std::string result = buf.str();
    if (result.size() == 1)
    {
        return "[]";
    }
    result.pop_back();
    result.append("]");
    return result;
}