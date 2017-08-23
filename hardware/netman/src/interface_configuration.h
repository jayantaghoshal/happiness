#ifndef _INTERFACE_CONFIGURATION_H_
#define _INTERFACE_CONFIGURATION_H_

#include <string>
#include <cstdint>
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
    int mtu = 0;
};

}
}

#endif
