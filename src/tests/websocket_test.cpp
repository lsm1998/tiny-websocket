//
// Created by Administrator on 2023/12/15.
//

#include <gtest/gtest.h>
#include <algorithm>
#include <netinet/in.h>
#include "base64.hpp"
#include "sha1.hpp"
#include "websocket_request.hpp"

TEST(websocket_test, PositiveInput)
{
    SHA1 sha;
    unsigned int message_digest[5];
    sha.Reset();


    sha << "6TS3Q1LykE60EDJ2Exflqw==";
    sha << WEBSOCKET_GUID;

    sha.Result(message_digest);
    for (unsigned int &i: message_digest)
    {
        i = htonl(i);
    }
    auto str = base64_encode(reinterpret_cast<const unsigned char *>(message_digest), 20);
    EXPECT_EQ(str, "fFBooB7FAkLlXgRSz0BT3v4hq5s=");
}
