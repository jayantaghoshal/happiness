/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_NET_SERIALIZER_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_NET_SERIALIZER_H_

#include <cstdint>
#include <vector>

namespace Connectivity {
class NetSerializer {
  public:
    NetSerializer(std::vector<std::uint8_t> &vector) : vector(vector) {}

    void write_uint8(std::uint8_t value);
    void write_uint16(std::uint16_t value);
    void write_uint32(std::uint32_t value);

  private:
    std::vector<std::uint8_t> &vector;
};
}

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_NET_SERIALIZER_H_
