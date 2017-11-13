/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "ipcommandbus/net_serializer.h"

namespace Connectivity {
TEST(NetSerializerTest, WriteUint8) {
    std::vector<std::uint8_t> vector;
    NetSerializer serializer(vector);

    serializer.write_uint8(0x12);
    EXPECT_EQ(1U, vector.size());
    serializer.write_uint8(0xfe);
    EXPECT_EQ(2U, vector.size());

    EXPECT_EQ(std::vector<std::uint8_t>({0x12, 0xfe}), vector);
}

TEST(NetSerializerTest, WriteUint16) {
    std::vector<std::uint8_t> vector;
    NetSerializer serializer(vector);

    serializer.write_uint16(0x1234);
    EXPECT_EQ(2U, vector.size());
    serializer.write_uint16(0xfedc);
    EXPECT_EQ(4U, vector.size());

    EXPECT_EQ(std::vector<std::uint8_t>({0x12, 0x34, 0xfe, 0xdc}), vector);
}

TEST(NetSerializerTest, WriteUint32) {
    std::vector<std::uint8_t> vector;
    NetSerializer serializer(vector);

    serializer.write_uint32(0x12345678);
    EXPECT_EQ(4U, vector.size());
    serializer.write_uint32(0xfedcba98);
    EXPECT_EQ(8U, vector.size());

    EXPECT_EQ(std::vector<std::uint8_t>({0x12, 0x34, 0x56, 0x78, 0xfe, 0xdc, 0xba, 0x98}), vector);
}
}
