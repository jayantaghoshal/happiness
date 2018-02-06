/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace vcc {
namespace netman {

struct InterfaceConfiguration {
    using vlan_conf = std::unordered_map<std::string, std::string>;

    std::string name;
    std::string ip_address;
    std::string netmask;
    std::string broadcast_address;
    std::string mac_address;
    std::vector<std::uint8_t> mac_address_bytes;
    std::uint32_t mtu = 0;
    std::vector<vlan_conf> vlan;
};

}  // namespace netman
}  // namespace vcc
