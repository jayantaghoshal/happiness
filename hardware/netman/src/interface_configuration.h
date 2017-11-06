/*
 * ===========================================================================
 * [2017]- Volvo Car Corporation
 * All Rights Reserved
 * LEGAL
 * NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
 * property of or licensed to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be
 * covered by patents or patent applications. This information is protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Volvo Car Corporation.
 */

#ifndef _INTERFACE_CONFIGURATION_H_
#define _INTERFACE_CONFIGURATION_H_

#include <cstdint>
#include <string>
#include <vector>

namespace vcc {
namespace netman {

struct InterfaceConfiguration {
    std::string name;
    std::string ip_address;
    std::string netmask;
    std::string broadcast_address;
    std::string mac_address;
    std::vector<std::uint8_t> mac_address_bytes;
    std::uint32_t mtu = 0;
};

}  // namespace netman
}  // namespace vcc

#endif  // _INTERFACE_CONFIGURATION_H_
