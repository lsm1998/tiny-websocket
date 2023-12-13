//
// Created by Administrator on 2023/12/10.
//

#include <gtest/gtest.h>
#include <algorithm>
#include "base64.hpp"
#include "sha1.hpp"

TEST(StringUtilTest_base64, PositiveInput)
{
    auto eStr1 = base64_encode("青龙偃月刀");
    EXPECT_EQ(eStr1, "6Z2S6b6Z5YGD5pyI5YiA");

    auto eStr2 = base64_encode("hello world");
    EXPECT_EQ(eStr2, "aGVsbG8gd29ybGQ=");

    auto eStr3 = base64_encode(
            "风急天高猿啸哀，渚清沙白鸟飞回。无边落木萧萧下，不尽长江滚滚来。万里悲秋常作客，百年多病独登台。艰难苦恨繁霜鬓，潦倒新停浊酒杯。");
    EXPECT_EQ(eStr3,
              "6aOO5oCl5aSp6auY54y/5ZW45ZOA77yM5ria5riF5rKZ55m96bif6aOe5Zue44CC5peg6L656JC95pyo6JCn6JCn5LiL77yM5LiN5bC96ZW/5rGf5rua5rua5p2l44CC5LiH6YeM5oKy56eL5bi45L2c5a6i77yM55m+5bm05aSa55eF54us55m75Y+w44CC6Imw6Zq+6Ium5oGo57mB6Zyc6ayT77yM5r2m5YCS5paw5YGc5rWK6YWS5p2v44CC");

    auto eStr4 = base64_decode("aGVsbG8gd29ybGQ=");
    EXPECT_EQ(eStr4, "hello world");
}

TEST(StringUtilTest_sha1, PositiveInput)
{
    SHA1 sha1;
    sha1
            << "风急天高猿啸哀，渚清沙白鸟飞回。无边落木萧萧下，不尽长江滚滚来。万里悲秋常作客，百年多病独登台。艰难苦恨繁霜鬓，潦倒新停浊酒杯。";
    unsigned message_digest_array[5];
    sha1.Result(message_digest_array);

    auto str = sha1.GetDigestString();
    std::transform(str.begin(),str.end(),str.begin(),::toupper);

    EXPECT_EQ(str, "8FDF0F1052E84A86A4D0FE831F18FC6C08916860");
}