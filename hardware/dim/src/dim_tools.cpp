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

#include "dim_tools.h"
#include <fcntl.h>
#include <array>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace vcc {
namespace dim {
namespace dim_tools {

std::string DimTools::ConvertHexDataToString(const std::array<uint8_t, 254>& data) {
    std::stringstream soutput;
    soutput << std::setfill('0');
    if (soutput.good()) {
        for (auto const& value : data) {
            soutput << std::hex << std::setw(2) << std::uppercase << static_cast<int>(value) << " ";
        }
    }
    return soutput.str();
}

std::vector<uint8_t> DimTools::ConvertStringToHexData(const std::string& str) {
    std::vector<uint8_t> result;
    for (auto& value : str) {
        result.push_back(static_cast<uint8_t>(value));
    }
    return result;
}

}  // namespace dim_tools
}  // namespace dim
}  // namespace vcc
