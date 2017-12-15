/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
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
