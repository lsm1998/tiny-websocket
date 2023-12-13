//
// Created by Administrator on 2023/12/13.
//

#include "base64.hpp"

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(const std::string &input)
{
    // 输入数据的指针
    const auto *bytes_to_encode = reinterpret_cast<const unsigned char *>(input.c_str());
    // 输入数据的长度
    size_t in_len = input.length();

    // 存储编码后的结果
    std::string encoded;

    // 对输入数据进行编码
    for (size_t i = 0; i < in_len; i += 3)
    {
        // 取三个字节
        uint32_t three_bytes = static_cast<uint32_t>(bytes_to_encode[i]) << 16;
        if (i + 1 < in_len) three_bytes |= static_cast<uint32_t>(bytes_to_encode[i + 1]) << 8;
        if (i + 2 < in_len) three_bytes |= bytes_to_encode[i + 2];

        // 将三个字节拆分为四个6位组，并转换为Base64字符
        encoded += base64_chars[(three_bytes >> 18) & 0x3F];
        encoded += base64_chars[(three_bytes >> 12) & 0x3F];
        encoded += (i + 1 < in_len) ? base64_chars[(three_bytes >> 6) & 0x3F] : '=';
        encoded += (i + 2 < in_len) ? base64_chars[three_bytes & 0x3F] : '=';
    }

    return encoded;
}

std::string base64_decode(const std::string &input)
{
    size_t in_len = input.size();
    int i = 0;
    int j;
    int in_ = 0;
    unsigned char charArray4[4], charArray3[3];
    std::string ret;

    while (in_len-- && (input[in_] != '=') && is_base64(input[in_]))
    {
        charArray4[i++] = input[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                charArray4[i] = base64_chars.find(charArray4[i]);
            }
            charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
            charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
            charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

            for (i = 0; (i < 3); i++)
            {
                ret += charArray3[i];
            }
            i = 0;
        }
    }
    if (i)
    {
        for (j = i; j < 4; j++)
        {
            charArray4[j] = 0;
        }
        for (j = 0; j < 4; j++)
        {
            charArray4[j] = base64_chars.find(charArray4[j]);
        }
        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];
        for (j = 0; (j < i - 1); j++) ret += charArray3[j];
    }
    return ret;
}