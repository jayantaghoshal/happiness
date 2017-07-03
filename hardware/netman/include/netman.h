#ifndef _VCC_NETMAN_NETMAN_H_
#define _VCC_NETMAN_NETMAN_H_

#include <string>

namespace vcc {
namespace netman {

struct InterfaceConfiguration {
    std::string name;
    std::string ip_address;
    std::string netmask;
    std::string mac_address;
    int mtu;
};

void LoadInterfaceConfiguration(InterfaceConfiguration &conf);

int OpenNetlinkSocket();

void CloseNetlinkSocket(const int socket_fd);

int BindNetlinkSocket(const int socket_fd);

void ListenForNetlinkEvents(const int socket_fd, const InterfaceConfiguration &interface_conf);

}
}

#endif
