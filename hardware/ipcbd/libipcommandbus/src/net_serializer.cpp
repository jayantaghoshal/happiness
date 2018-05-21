/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdint>
#include <vector>

#include "ipcommandbus/net_serializer.h"

namespace Connectivity {
void NetSerializer::write_uint8(std::uint8_t value) {
    vector.push_back(static_cast<std::uint8_t>(value));
}

void NetSerializer::write_uint16(std::uint16_t value) {
    write_uint8((value & 0xff00) >> 8);
    write_uint8(value & 0x00ff);
}

void NetSerializer::write_uint32(std::uint32_t value) {
    write_uint8((value & 0xff000000) >> 24);
    write_uint8((value & 0x00ff0000) >> 16);
    write_uint8((value & 0x0000ff00) >> 8);
    write_uint8(value & 0x000000ff);
}
}  // namespace Connectivity
