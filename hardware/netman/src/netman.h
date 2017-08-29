#ifndef _NETMAN_H_
#define _NETMAN_H_

#include <string>
#include <vector>

#include "netlink_event_handler.h"
#include "interface_configuration.h"

namespace vcc {
namespace netman {

void LoadInterfaceConfiguration(std::vector<InterfaceConfiguration> &interface_configurations);

void PrintInterfaceConfiguration(const std::string context, const InterfaceConfiguration &conf);

void SetupInterface(const std::vector<InterfaceConfiguration> &interface_configurations);

bool SetupInterface(const char* interface_name,
                    const std::vector<uint8_t> &mac_address,
                    const char* ip_addr,
                    const char* netmask,
                    const char* broadcast_addr,
                    const uint32_t mtu);

}
}

#endif
