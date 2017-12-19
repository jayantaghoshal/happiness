/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef DIMTOOLS_H
#define DIMTOOLS_H

#include <array>
#include <string>
#include <vector>

namespace vcc {
namespace dim {
namespace dim_tools {

class DimTools {
  public:
    static std::string ConvertHexDataToString(const std::array<uint8_t, 254>& data);
    static std::vector<uint8_t> ConvertStringToHexData(const std::string& str);
};

}  // namespace dim_tools
}  // namespace dim
}  // namespace vcc
#endif
