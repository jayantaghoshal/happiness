/*
*  ===========================================================================
*  [2017]- Volvo Car Corporation
*  All Rights Reserved
*  LEGAL
*  NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
* property of or licensed
*  to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be covered by patents or
* patent applications.
*  This information is protected by trade secret or copyright law. Dissemination of this information or reproduction of
* this material is
*  strictly forbidden unless prior written permission is obtained from Volvo Car Corporation.
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
