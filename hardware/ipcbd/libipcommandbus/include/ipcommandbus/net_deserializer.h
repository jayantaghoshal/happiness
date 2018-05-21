/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Connectivity {
class NetDeserializer {
  public:
    NetDeserializer(const std::vector<std::uint8_t>& vector) : NetDeserializer(vector, 0) {}

    NetDeserializer(const std::vector<std::uint8_t>& vector, std::vector<std::uint8_t>::size_type position)
        : vector(vector), position(position) {}

    std::size_t bytes_available() const;

    std::uint8_t read_uint8();
    std::uint16_t read_uint16();
    std::uint32_t read_uint32();

  private:
    const std::vector<std::uint8_t>& vector;
    std::vector<std::uint8_t>::size_type position;
};
}  // namespace Connectivity
