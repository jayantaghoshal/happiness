#include "netman_event_handler.h"

#include <cutils/log.h>
#include <string>
#include <vector>

#include "netutils.h"

#define LOG_TAG "Netmand"

namespace vcc {
namespace netman {

NetmanEventHandler::NetmanEventHandler(const std::vector<InterfaceConfiguration> &interface_configurations)
    : interface_configurations_(interface_configurations) {}

void NetmanEventHandler::HandleEvent(const char *uevent, const int message_length) {
    if (!uevent) return;

    const char *const MOVE_ACTION = "move@";
    const char *const ADD_ACTION = "add@";

    // Currently move@ and add@ event are handled. Can be extended as per need
    if (message_length < std::min(static_cast<int>(std::string(MOVE_ACTION).length()),
                                  static_cast<int>(std::string(ADD_ACTION).length()))) {
        return;
    }

    if (std::string(uevent).find(MOVE_ACTION) == std::string::npos &&
        std::string(uevent).find(ADD_ACTION) == std::string::npos) {
        return;
    }

    // advance buffer past first segment
    int index = strlen(uevent) + 1;
    bool handle_new_link_evt = false;
    std::string interface;

    while (index < message_length) {
        std::string line(uevent + index);

        // see if it is a networking event
        if (line.find(ATTR_IFINDEX) != std::string::npos) {
            handle_new_link_evt = true;
        }

        if (line.find(ATTR_INTERFACE) != std::string::npos) {
            interface = ExtractAttribute(line, ATTR_INTERFACE);
        }

        index += line.length() + 1;
    }

    if (handle_new_link_evt) {
        for (const auto &ic : interface_configurations_) {
            if (interface == ic.name) {
                SetupInterface(ic.name.c_str(), ic.mac_address_bytes, ic.ip_address.c_str(), ic.netmask.c_str(),
                               ic.broadcast_address.c_str(), ic.mtu);
            }
        }
    }
}

}  // namespace netman
}  // namespace vcc
