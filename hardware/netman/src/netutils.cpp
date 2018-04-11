/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "netutils.h"

#include <vcc/localconfig.h>

#include <arpa/inet.h>
#include <ifaddrs.h>

// NOTE: Quick fix for using external/iptable/libiptc which is only
// purposed to be used by external/iptables. this resulted in a messy include order
// which hides __KERNEL_DIV_ROUND_UP in original linux/kenel.h
#ifndef __KERNEL_DIV_ROUND_UP
#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))
#endif

#include <linux/ethtool.h>

#include <linux/if_arp.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <cerrno>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <system_error>

#define LOG_TAG "Netmand"
#include <cutils/log.h>

namespace {

void ValidateReturnStatus(const int command_status, const std::string& what) {
    if ((command_status < 0) || !WIFEXITED(command_status) || WEXITSTATUS(command_status) != EXIT_SUCCESS) {
        throw std::system_error(WEXITSTATUS(command_status), std::system_category(), what.c_str());
    }
}

bool WriteFile(const std::string& path, const std::string& text) {
    std::fstream file(path);

    if (file.is_open()) {
        file << text;
    }
    return file.good();
}

bool SetProxyArp(const char* interface_name) {
    ALOGV("%s: Setting proxy_arp = 1", interface_name);
    return WriteFile(std::string("/proc/sys/net/ipv4/conf/") + interface_name + "/proxy_arp", "1");
}

// Helper functions
void ConvertMacAddress(const std::string& mac_address, std::vector<uint8_t>& mac_address_out) {
    mac_address_out.resize(6);

    std::stringstream address(mac_address, std::ios_base::in);

    address >> std::hex;

    int byte = 0;
    char skip_byte = 0;
    for (int pos = 0; pos <= 5; ++pos) {
        address >> byte >> skip_byte;
        mac_address_out[pos] = byte;
    }
}

bool IsMacAddressCorrect(const std::vector<uint8_t>& mac_address, const char* interface_name) {
    struct ifreq ifr_req;
    memset(&ifr_req, 0, sizeof(struct ifreq));
    std::strncpy(ifr_req.ifr_name, interface_name, IFNAMSIZ);

    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (inet_sock_fd == -1) {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");

        return false;
    }

    if (ioctl(inet_sock_fd, SIOCGIFHWADDR, &ifr_req) == -1) {
        ALOGE("Failed to get flags for %s interface", interface_name);

        close(inet_sock_fd);
        return false;
    }

    if (ifr_req.ifr_hwaddr.sa_family != ARPHRD_ETHER) {
        ALOGE("not an Ethernet interface");
    }

    const char* received_mac = static_cast<char*>(ifr_req.ifr_hwaddr.sa_data);

    bool match = true;
    for (int pos = 0; pos <= 5; ++pos) {
        if (received_mac[pos] != mac_address[pos]) {
            match = false;
            break;
        }
    }

    close(inet_sock_fd);
    return match;
}

std::uint32_t ethtool_cmd_speed_(const struct ethtool_cmd* ep) { return (ep->speed_hi << 16) | ep->speed; }

bool IsLinkSpeedCorrect(const std::string& interface_name) {
    int sock;
    struct ifreq ifr;
    struct ethtool_cmd edata;
    int rc;

    sock = socket(PF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_IP);
    if (sock < 0) {
        return false;
    }

    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ);
    ifr.ifr_data = &edata;

    edata.cmd = ETHTOOL_GSET;

    rc = ioctl(sock, SIOCETHTOOL, &ifr);
    if (rc < 0) {
        ALOGE("%s: ioctl call to get link speed failed", interface_name.c_str());
        close(sock);
        return false;
    }

    // TODO (Patrik Moberg): Hard coded link speed, make Local Config a rainy day.
    switch (ethtool_cmd_speed_(&edata)) {
        case SPEED_100:
            close(sock);
            return true;
        default:
            close(sock);
            return false;
    }

    close(sock);
    return false;
}

inline void ethtool_cmd_speed_set_(struct ethtool_cmd* ep, __u32 speed) {
    ep->speed = static_cast<std::uint16_t>(speed & 0xFFFF);
    ep->speed_hi = static_cast<std::uint16_t>(speed >> 16);
}

bool SetLinkSpeed(const std::string& interface_name) {
    int sock;
    struct ifreq ifr;
    struct ethtool_cmd edata;
    int rc;

    sock = socket(PF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_IP);
    if (sock < 0) {
        return false;
    }

    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ);
    ifr.ifr_data = &edata;

    // Some drivers does not allow for these operations. See ethtool.h for more instructions.
    edata.cmd = ETHTOOL_GSET;

    rc = ioctl(sock, SIOCETHTOOL, &ifr);
    if (rc < 0) {
        ALOGE("SetLinkSpeed NOT successful (Get). %s", std::strerror(errno));
        close(sock);
        return false;
    }

    edata.cmd = ETHTOOL_SSET;
    ethtool_cmd_speed_set_(&edata, SPEED_100);
    edata.duplex = DUPLEX_FULL;
    edata.autoneg = AUTONEG_DISABLE;

    rc = ioctl(sock, SIOCETHTOOL, &ifr);
    if (rc < 0) {
        close(sock);
        ALOGE("SetLinkSpeed NOT successful (Set). %s", std::strerror(errno));
        return false;
    }

    ALOGD("SetLinkSpeed successful.");
    close(sock);
    return true;
}

bool IsInterfaceUp(const char* interface_name) {
    std::ifstream istr(std::string("/sys/class/net/") + interface_name + "/flags");
    if (!istr.good()) {
        ALOGE("%s: Failed to read sysfs for interface", interface_name);
        return false;
    }

    int if_flags;
    istr >> std::hex >> if_flags;
    if (!istr.good()) {
        ALOGE("%s: Invalid argument for interface flags", interface_name);
        return false;
    }

    return (if_flags & IFF_UP) != 0;
}

std::string GetIpAddress(const std::string& interface_name) {
    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (inet_sock_fd == -1) {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        throw std::system_error(EFAULT, std::system_category());
    }

    struct ifreq ifr;
    std::memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_addr.sa_family = AF_INET;
    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ);

    // Get IP address
    if (ioctl(inet_sock_fd, SIOCGIFADDR, &ifr) == -1) {
        ALOGE("%s: ioctl call get IP address failed. Error is [%s]", interface_name.c_str(), strerror(errno));
        close(inet_sock_fd);
        return "";
    }

    close(inet_sock_fd);

    return inet_ntoa(reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr)->sin_addr);
}

std::string GetNetmask(const std::string& interface_name) {
    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (inet_sock_fd == -1) {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        throw std::system_error(EFAULT, std::system_category());
    }

    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;

    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ);

    // Get netmask
    if (ioctl(inet_sock_fd, SIOCGIFNETMASK, &ifr) == -1) {
        ALOGE("%s: ioctl call get netmask failed. Error is [%s]", interface_name.c_str(), strerror(errno));
        close(inet_sock_fd);
        return "";
    }

    close(inet_sock_fd);

    return inet_ntoa(reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_netmask)->sin_addr);
}

std::string GetBroadcastAddress(const std::string& interface_name) {
    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (inet_sock_fd == -1) {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        throw std::system_error(EFAULT, std::system_category());
    }

    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;

    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ);

    // Get netmask
    if (ioctl(inet_sock_fd, SIOCGIFBRDADDR, &ifr) == -1) {
        ALOGE("%s: ioctl call get broadcast failed. Error is [%s]", interface_name.c_str(), strerror(errno));
        close(inet_sock_fd);
        return "";
    }

    close(inet_sock_fd);
    return inet_ntoa(reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_broadaddr)->sin_addr);
}

bool SetBroadcastAddress(int inet_sock_fd, const char* interface_name, const char* broadcast_address) {
    struct sockaddr_in sin_addr {
        .sin_family = AF_INET, .sin_port = htons(0)
    };

    // safe to ignore return value as ip address is a const string in correct format
    inet_aton(broadcast_address, reinterpret_cast<struct in_addr*>(&sin_addr.sin_addr.s_addr));

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    memcpy(&ifr.ifr_broadaddr, &sin_addr, sizeof(struct sockaddr));
    std::strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);

    // Set broadcast address
    if (ioctl(inet_sock_fd, SIOCSIFBRDADDR, &ifr) == -1) {
        ALOGE("ioctl call set broadcast address failed. Error is [%s]", strerror(errno));
        return false;
    }

    return true;
}

bool SetNetmask(int skfd, const char* intf, const char* newmask) {
    struct ifreq ifr;
    auto* sin = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr);
    memset(&ifr, 0, sizeof(ifr));
    sin->sin_family = AF_INET;
    if (inet_pton(AF_INET, newmask, &sin->sin_addr) <= 0) {
        ALOGE("failed to convert netmask\n");
        return false;
    }

    std::strncpy(ifr.ifr_name, intf, IFNAMSIZ);
    if (ioctl(skfd, SIOCSIFNETMASK, &ifr) == -1) {
        ALOGE("could not read interface %s\n", intf);
        return false;
    }

    return true;
}

bool SetIpAddress(const char* interface_name, const char* ip_addr, const char* netmask, const char* broadcast_address) {
    // Open AF_INET socket
    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (inet_sock_fd == -1) {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        return false;
    }

    if (std::strcmp(GetIpAddress(interface_name).c_str(), ip_addr) != 0) {
        struct sockaddr_in sin_addr {
            .sin_family = AF_INET, .sin_port = htons(0)
        };

        // safe to ignore return value as ip address is a const string in correct format
        inet_aton(ip_addr, reinterpret_cast<struct in_addr*>(&sin_addr.sin_addr.s_addr));

        struct ifreq ifr;
        std::memset(&ifr, 0, sizeof(struct ifreq));
        std::memcpy(&ifr.ifr_addr, &sin_addr, sizeof(struct sockaddr));
        std::strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);

        // Set IP address
        if (ioctl(inet_sock_fd, SIOCSIFADDR, &ifr) == -1) {
            ALOGE("%s: ioctl call set ip address %s failed for device. Error is [%s]",
                  interface_name,
                  ip_addr,
                  strerror(errno));
            close(inet_sock_fd);
            return false;
        }
        ALOGV("%s: Ip address set to %s", interface_name, ip_addr);
    } else {
        ALOGV("%s: Ip address already set", interface_name);
    }

    if (std::strcmp(GetBroadcastAddress(interface_name).c_str(), broadcast_address) != 0) {
        if (!SetBroadcastAddress(inet_sock_fd, interface_name, broadcast_address)) {
            ALOGE("ioctl call to set broadcast address failed. Error is [%s]", strerror(errno));
            close(inet_sock_fd);
            return false;
        }
        ALOGV("%s: Broadcast address set to %s", interface_name, broadcast_address);
    } else {
        ALOGV("%s: Broadcast address already set", interface_name);
    }

    if (std::strcmp(GetNetmask(interface_name).c_str(), netmask) != 0) {
        if (!SetNetmask(inet_sock_fd, interface_name, netmask)) {
            ALOGE("ioctl call to set netmask failed. Error is [%s]", strerror(errno));
            close(inet_sock_fd);
            return false;
        }
        ALOGV("%s: Netmask set to %s", interface_name, netmask);
    } else {
        ALOGV("%s: Netmask already set", interface_name);
    }

    close(inet_sock_fd);
    return true;
}

bool SetMtu(const uint32_t mtu, const char* interface_name) {
    std::ofstream ostr(std::string("/sys/class/net/") + interface_name + "/mtu");
    if (!ostr.good()) {
        ALOGE("%s: Failed to read sysfs for interface", interface_name);
        return false;
    }

    ostr << mtu;
    if (!ostr.good()) {
        ALOGE("%s: Failed to set mtu for interface", interface_name);
    }

    return ostr.good();
}

std::uint32_t GetMtu(const std::string& interface_name) {
    std::ifstream istr(std::string("/sys/class/net/") + interface_name + "/mtu");
    if (!istr.good()) {
        ALOGE("%s: Failed to read sysfs for interface", interface_name.c_str());
        return 0;
    }

    int mtu;
    istr >> mtu;
    if (!istr.good()) {
        ALOGE("%s: Failed to get mtu for interface", interface_name.c_str());
        return 0;
    }

    return mtu;
}

bool SetMacAddress(const std::vector<uint8_t>& mac_address, const char* interface_name) {
    struct ifreq ifr_mac;
    int sockfd;

    for (int pos = 0; pos <= 5; ++pos) {
        ifr_mac.ifr_hwaddr.sa_data[pos] = mac_address[pos];
    }

    sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (sockfd == -1) {
        ALOGE("Unable to open socket for set MAC address for device. %s", interface_name);
        return false;
    }

    std::strncpy(ifr_mac.ifr_name, interface_name, IFNAMSIZ);
    ifr_mac.ifr_hwaddr.sa_family = ARPHRD_ETHER;

    // Set MAC address
    std::string printable_mac_address(mac_address.begin(), mac_address.end());
    if (ioctl(sockfd, SIOCSIFHWADDR, &ifr_mac) == -1) {
        close(sockfd);
        ALOGE("Unable to set MAC address (%s) for %s, errno %s.",
              printable_mac_address.c_str(),
              interface_name,
              strerror(errno));
        return false;
    }

    ALOGV("%s: MAC address set to %s", interface_name, printable_mac_address.c_str());
    close(sockfd);
    return true;
}

}  // namespace

namespace vcc {
namespace netman {

void PrintInterfaceConfiguration(const std::string& context, const InterfaceConfiguration& conf) {
    ALOGV("--------------------------------------------------------");
    ALOGV("Interface configuration: %s", context.c_str());
    ALOGV("Interface name: %s", conf.name.c_str());
    ALOGV("Mac address: %s", conf.mac_address.c_str());
    ALOGV("Mac address bytes: %02x:%02x:%02x:%02x:%02x:%02x",
          conf.mac_address_bytes[0],
          conf.mac_address_bytes[1],
          conf.mac_address_bytes[2],
          conf.mac_address_bytes[3],
          conf.mac_address_bytes[4],
          conf.mac_address_bytes[5]);
    ALOGV("IP address: %s", conf.ip_address.c_str());
    ALOGV("Broadcast address: %s", conf.broadcast_address.c_str());
    ALOGV("Netmask: %s", conf.netmask.c_str());
    ALOGV("MTU: %i", conf.mtu);
}

void LoadInterfaceConfiguration(std::vector<InterfaceConfiguration>* interface_configurations,
                                const vcc::LocalConfigReaderInterface* lcfg) {
    const std::vector<std::string> interface_names = {"apix0", "tcam0", "meth0"};
    for (auto& name : interface_names) {
        InterfaceConfiguration conf;
        conf.name = name;
        conf.ip_address = lcfg->GetString(name, "ip-address");
        conf.netmask = lcfg->GetString(name, "netmask");
        conf.mac_address = lcfg->GetString(name, "mac-address");
        ConvertMacAddress(conf.mac_address, conf.mac_address_bytes);
        conf.broadcast_address = lcfg->GetString(name, "broadcast-address");
        conf.mtu = static_cast<std::uint32_t>(lcfg->GetInt(name, "mtu"));
        try {
            auto string_array = lcfg->GetStringArray(name, "vlan");
            for (auto& str : string_array) {
                // strip out any whitespace characters
                str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());

                std::istringstream str_stream(str);
                std::string attr;
                InterfaceConfiguration::vlan_conf entry;

                while (std::getline(str_stream, attr, ',') && !attr.empty()) {
                    std::string key, value;
                    std::getline(std::getline(std::istringstream(attr), key, '=') >> std::ws, value);
                    entry[key] = value;
                }
                conf.vlan.push_back(entry);
            }
        } catch (const std::runtime_error& e) {
            ALOGE("%s. Skipping...", e.what());
        }
        interface_configurations->push_back(conf);
    }
}

bool BringInterfaceUp(const char* interface_name) {
    std::ifstream istr(std::string("/sys/class/net/") + interface_name + "/flags");
    if (!istr.good()) {
        ALOGE("%s: Failed to read sysfs for interface", interface_name);
        return false;
    }

    int if_flags;
    istr >> std::hex >> if_flags;
    if (!istr.good()) {
        ALOGE("%s: Invalid argument for interface flags", interface_name);
        return false;
    }

    if_flags |= (IFF_UP | IFF_RUNNING);

    std::ofstream ostr(std::string("/sys/class/net/") + interface_name + "/flags");
    if (!ostr.good()) {
        ALOGE("Failed to bring %s interface up", interface_name);
        return false;
    }
    ostr << "0x" << std::hex << if_flags;

    return ostr.good();
}

bool TakeInterfaceDown(const char* interface_name) {
    std::ifstream istr(std::string("/sys/class/net/") + interface_name + "/flags");
    if (!istr.good()) {
        ALOGE("%s: Failed to read sysfs for interface", interface_name);
        return false;
    }

    int if_flags;
    istr >> std::hex >> if_flags;
    if (!istr.good()) {
        ALOGE("%s: Invalid argument for interface flags", interface_name);
        return false;
    }

    if_flags &= ~(IFF_UP | IFF_RUNNING);

    std::ofstream ostr(std::string("/sys/class/net/") + interface_name + "/flags");
    if (!ostr.good()) {
        ALOGE("Failed to take %s interface down", interface_name);
        return false;
    }
    ostr << "0x" << std::hex << if_flags;

    return ostr.good();
}

void MoveNetworkInterfaceToNamespace(const std::string& current_name,
                                     const std::string& ns,
                                     const std::string& new_name) {
    std::stringstream move_network_interface_cmd;

    move_network_interface_cmd << "/vendor/bin/ip link set dev " << current_name;

    if (!new_name.empty()) move_network_interface_cmd << " name " << new_name;

    if (!ns.empty()) move_network_interface_cmd << " netns " << ns;

    // TODO (Abhijeet Shirolikar): system calls involves command processor and so is vunerable to injection attacks
    // Refactor code below to use exec family function together with fork and pipe
    ValidateReturnStatus(system(move_network_interface_cmd.str().c_str()),  // NOLINT
                         std::string("Failed to move ") + current_name);
}

bool SetupInterface(const char* interface_name,
                    const std::vector<uint8_t>& mac_address,
                    const char* ip_addr,
                    const char* netmask,
                    const char* broadcast_addr,
                    const uint32_t mtu) {
    ALOGD("%s: Setting configuration for network interface", interface_name);

    // TODO (Patrik Moberg): Remove hard coded implementation. General refactoring needed.
    if (std::strcmp(interface_name, "tcam0") == 0 && !IsLinkSpeedCorrect(interface_name)) {
        SetLinkSpeed(interface_name);
        ALOGV("%s: Link speed set to 100Mbit", interface_name);
    }

    if (!IsMacAddressCorrect(mac_address, interface_name)) {
        if (IsInterfaceUp(interface_name)) {
            TakeInterfaceDown(interface_name);
        }

        if (!SetMacAddress(mac_address, interface_name)) {
            ALOGE("Failed to set MAC address for %s!", interface_name);
        }
    } else {
        ALOGV("%s: MAC address already set", interface_name);
    }

    // Arp proxy settings
    if (strcmp(interface_name, "meth0") == 0 || strcmp(interface_name, "tcam0") == 0) {
        if (!SetProxyArp(interface_name)) {
            ALOGE("Failed to set proxy arp for %s!", interface_name);
        }
    }

    // Set ip adress if ethernet interface is up already...
    if (!SetIpAddress(interface_name, ip_addr, netmask, broadcast_addr)) {
        ALOGE("Failed to configure IP address for %s!", interface_name);
    }

    if (GetMtu(interface_name) != mtu) {
        if (!SetMtu(mtu, interface_name)) {
            ALOGE("Failed to set MTU for %s!", interface_name);
        }
    } else {
        ALOGV("%s: MTU already set", interface_name);
    }

    // All well so far; bring the interface up
    if (!IsInterfaceUp(interface_name)) {
        if (!BringInterfaceUp(interface_name)) {
            ALOGE("Failed to bring ethernet port up for %s!", interface_name);
        }
    }

    return true;
}

void SetupVLan(const InterfaceConfiguration& interface_configuration) {
    for (auto& entry : interface_configuration.vlan) {
        try {
            // TODO (Abhi) Change below implementation to use netlink library instead
            std::string cmd = "/vendor/bin/ip link add link " + interface_configuration.name + " name " +
                              entry.at("name") + " type vlan id " + entry.at("id") + " egress 0:" + entry.at("prio");

            try {
                ValidateReturnStatus(std::system(cmd.c_str()),
                                     std::string("Failed to setup Vlan interface ") + entry.at("name"));
            } catch (const std::system_error& e) {
                if (e.code().value() != ENOENT) throw;
            }

            if (!SetIpAddress(entry.at("name").c_str(),
                              entry.at("ip-address").c_str(),
                              entry.at("netmask").c_str(),
                              entry.at("broadcast-address").c_str())) {
                ALOGE("Failed to configure IP address for: %s", entry.at("name").c_str());
                return;
            }

            if (!BringInterfaceUp(entry.at("name").c_str())) {
                ALOGE("Failed to bring up interface: %s!", entry.at("name").c_str());
                return;
            }
        } catch (const std::out_of_range& e) {
            ALOGE("Error: %s!", e.what());
        }
    }
}

}  // namespace netman
}  // namespace vcc
