#ifndef _NETMAN_H_
#define _NETMAN_H_

#include <string>
#include <vector>

#include "netlink_event_handler.h"
#include "interface_configuration.h"

namespace vcc {
namespace netman {

void LoadInterfaceConfiguration(InterfaceConfiguration &conf);

void PrintInterfaceConfiguration(const std::string context, const InterfaceConfiguration &conf);

bool SetupInterface(const char* interface_name,
                    const std::vector<uint8_t> &mac_address,
                    const char* ip_addr,
                    const char* netmask,
                    const char* broadcast_addr,
                    const uint32_t mtu);

}
}

#endif
