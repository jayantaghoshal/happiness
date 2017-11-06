/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ipcommandbus/net_deserializer.h"

namespace Connectivity {
TEST(NetDeserializerTest, BytesAvailable) {
    std::vector<std::uint8_t> vector;

    EXPECT_EQ(0U, NetDeserializer(vector, 0).bytes_available());
    EXPECT_EQ(0U, NetDeserializer(vector, 1).bytes_available());

    vector.resize(2);
    EXPECT_EQ(2U, NetDeserializer(vector, 0).bytes_available());
    EXPECT_EQ(1U, NetDeserializer(vector, 1).bytes_available());
    EXPECT_EQ(0U, NetDeserializer(vector, 2).bytes_available());
    EXPECT_EQ(0U, NetDeserializer(vector, 3).bytes_available());
}

TEST(NetDeserializerTest, ReadUint8) {
    std::vector<std::uint8_t> vector({0x12, 0xfe});
    NetDeserializer deserializer(vector);

    EXPECT_EQ(0x12, deserializer.read_uint8());
    EXPECT_EQ(0xfe, deserializer.read_uint8());
    EXPECT_EQ(0x00, deserializer.read_uint8());
}

TEST(NetDeserializerTest, ReadUint16) {
    std::vector<std::uint8_t> vector({0x12, 0x34, 0xfe, 0xdc});
    NetDeserializer deserializer(vector);

    EXPECT_EQ(0x1234, deserializer.read_uint16());
    EXPECT_EQ(0xfedc, deserializer.read_uint16());
    EXPECT_EQ(0x0000, deserializer.read_uint16());
}

TEST(NetDeserializerTest, ReadUint32) {
    std::vector<std::uint8_t> vector({0x12, 0x34, 0x56, 0x78, 0xfe, 0xdc, 0xba, 0x98});
    NetDeserializer deserializer(vector);

    EXPECT_EQ(0x12345678U, deserializer.read_uint32());
    EXPECT_EQ(0xfedcba98U, deserializer.read_uint32());
    EXPECT_EQ(0x00000000U, deserializer.read_uint32());
}
}
