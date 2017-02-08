/*!
 * Title ---- tests/BufferTest.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include "BufferTest.hpp"

namespace tests { // Begin test namespace

TEST(BufferTest, constructor_empty)
{
    Buffer buffer;

    ASSERT_EQ(0u, buffer.size());
}

TEST(BufferTest, constructor_size)
{
    Buffer buffer(10);

    ASSERT_EQ(10u, buffer.size());
}

TEST(BufferTest, constructor_range)
{
    std::string data = "abcde";

    Buffer buffer(data.cbegin(), data.cend());

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, constructor_list)
{
    Buffer buffer({ 'a', 'b', 'c', 'd', 'e' });

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, fromArray)
{
    std::uint8_t list[] = { 'a', 'b', 'c', 'd', 'e' };

    auto buffer = Buffer::fromArray(list, 5);

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, toArray)
{
    auto buffer = Buffer::fromString("abcde");
    auto result = buffer.toArray();

    ASSERT_EQ('a', result[0]);
    ASSERT_EQ('b', result[1]);
    ASSERT_EQ('c', result[2]);
    ASSERT_EQ('d', result[3]);
    ASSERT_EQ('e', result[4]);
}

TEST(BufferTest, fromString)
{
    auto buffer = Buffer::fromString("abcde");

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, toString)
{
    auto buffer = Buffer::fromString("abcde");
    auto result = buffer.toString();

    ASSERT_EQ(5u, result.length());
    ASSERT_EQ("abcde", result);
}

TEST(BufferTest, fromHex_String)
{
    auto buffer = Buffer::fromHex<std::string>("6162636465");

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, toHex_String)
{
    auto buffer = Buffer::fromString("abcde");
    auto result = buffer.toHex<std::string>();

    ASSERT_EQ(10u, result.length());
    ASSERT_EQ("6162636465", result);
}

TEST(BufferTest, fromHex_Buffer)
{
    auto buffer = Buffer::fromHex<Buffer>(
        Buffer::fromString("6162636465")
    );

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, toHex_Buffer)
{
    auto buffer = Buffer::fromString("abcde");
    auto result = buffer.toHex<Buffer>();

    ASSERT_EQ(10u, result.size());
    ASSERT_EQ(Buffer::fromString("6162636465"), result);
}

TEST(BufferTest, fromBase64_String)
{
    auto buffer = Buffer::fromBase64<std::string>("YWJjZGU=");

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, toBase64_String)
{
    auto buffer0 = Buffer::fromString("abc");
    auto result0 = buffer0.toBase64<std::string>();

    ASSERT_EQ(4u, result0.size());
    ASSERT_EQ("YWJj", result0);

    auto buffer1 = Buffer::fromString("abcd");
    auto result1 = buffer1.toBase64<std::string>();

    ASSERT_EQ(8u, result1.size());
    ASSERT_EQ("YWJjZA==", result1);

    auto buffer2 = Buffer::fromString("abcde");
    auto result2 = buffer2.toBase64<std::string>();

    ASSERT_EQ(8u, result2.size());
    ASSERT_EQ("YWJjZGU=", result2);
}

TEST(BufferTest, fromBase64_Buffer)
{
    auto buffer = Buffer::fromBase64<Buffer>(
        Buffer::fromString("YWJjZGU=")
    );

    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ("abcde", buffer.toString());
}

TEST(BufferTest, toBase64_Buffer)
{
    auto buffer0 = Buffer::fromString("abc");
    auto result0 = buffer0.toBase64<Buffer>();

    ASSERT_EQ(4u, result0.size());
    ASSERT_EQ("YWJj", result0.toString());

    auto buffer1 = Buffer::fromString("abcd");
    auto result1 = buffer1.toBase64<Buffer>();

    ASSERT_EQ(8u, result1.size());
    ASSERT_EQ("YWJjZA==", result1.toString());

    auto buffer2 = Buffer::fromString("abcde");
    auto result2 = buffer2.toBase64<Buffer>();

    ASSERT_EQ(8u, result2.size());
    ASSERT_EQ("YWJjZGU=", result2.toString());
}

TEST(BufferTest, fromRandom)
{
    auto buffer = Buffer::fromRandom(25);

    ASSERT_EQ(25u, buffer.size());
}

TEST(BufferTest, DISABLED_destructor)
{
    Buffer::value_type *ptr;

    {
        auto buffer = Buffer::fromString("abcde");
        ptr = buffer.data();
    }

    ASSERT_EQ('\0', ptr[0]);
    ASSERT_EQ('\0', ptr[1]);
    ASSERT_EQ('\0', ptr[2]);
    ASSERT_EQ('\0', ptr[3]);
    ASSERT_EQ('\0', ptr[4]);
}

} // End of test namespace