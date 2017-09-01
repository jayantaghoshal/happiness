#include <arpa/inet.h>

#include <cstddef>
#include <cutils/log.h>

#include <errno.h>
#include <ios>

#include <linux/if_arp.h>
#include <linux/rtnetlink.h>

#include <net/if.h>

#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <vector>
#include <unistd.h>

#include <vcc/localconfig.h>

#include "netman.h"

namespace vcc {
namespace netman {

#define  LOG_TAG    "Netmand"

/* Declarations */

void ConvertMacAddress(const std::string &mac_address,
                       std::vector<uint8_t> &mac_address_out);

/** Public Functions **/


void PrintInterfaceConfiguration(const std::string context,
                                 const InterfaceConfiguration &conf)
{
    ALOGI("--------------------------------------------------------");
    ALOGI("Interface configuration: %s", context.c_str());
    ALOGI("Interface name: %s", conf.name.c_str());
    ALOGI("Mac address: %s", conf.mac_address.c_str());
    ALOGI("Mac address bytes: %02x:%02x:%02x:%02x:%02x:%02x",
          conf.mac_address_bytes[0], conf.mac_address_bytes[1], conf.mac_address_bytes[2],
          conf.mac_address_bytes[3], conf.mac_address_bytes[4], conf.mac_address_bytes[5]);
    ALOGI("IP address: %s", conf.ip_address.c_str());
    ALOGI("Broadcast address: %s", conf.broadcast_address.c_str());
    ALOGI("Netmask: %s", conf.netmask.c_str());
    ALOGI("MTU: %i", conf.mtu);
}

void LoadInterfaceConfiguration(std::vector<InterfaceConfiguration> &interface_configurations)
{
    const std::vector<std::string> interface_names = {"eth0", "eth1", "meth0"};
    for (auto &name : interface_names) {
        InterfaceConfiguration conf;
        conf.name = vcc::localconfig::GetString(name + ".name");
        conf.ip_address = vcc::localconfig::GetString(name + ".ip-address");
        conf.netmask = vcc::localconfig::GetString(name + ".netmask");
        conf.mac_address = vcc::localconfig::GetString(name + ".mac-address");
        ConvertMacAddress(conf.mac_address, conf.mac_address_bytes);
        conf.broadcast_address = vcc::localconfig::GetString(name + ".broadcast-address");
        conf.mtu = (uint32_t)vcc::localconfig::GetInt(name + ".mtu");
        interface_configurations.push_back(conf);
    }
}

/** Private Functions */

void ConvertMacAddress(const std::string &mac_address, std::vector<uint8_t> &mac_address_out)
{
    int byte;
    char skip_byte;

    mac_address_out.resize(6);

    std::stringstream address(mac_address, std::ios_base::in);

    address >> std::hex;

    for (int pos = 0; pos <= 5; ++pos)
    {
        address >> byte >> skip_byte;

        mac_address_out[pos] = byte;
    }
}

// Helper functions
static bool InterfaceExists(const char* interface_name)
{
    //TODO: Find a better way to check if interface exists... Checking if we can retreive IFF flags maybe isn't the best way?!

    struct ifreq ifr_req;
    memset(&ifr_req, 0, sizeof(struct ifreq));
    strcpy(ifr_req.ifr_name, interface_name);

    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock_fd == -1)
    {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");

        return false;
    }

    if (ioctl(inet_sock_fd, SIOCGIFHWADDR, &ifr_req) == -1)
    {
        ALOGE("%s: Failed to get flags for interface", interface_name);

        close(inet_sock_fd);
        return false;
    }

    close(inet_sock_fd);
    return true;
}

static bool IsMacAddressCorrect(const std::vector<uint8_t> &mac_address, const char* interface_name)
{
    struct ifreq ifr_req;
    memset(&ifr_req, 0, sizeof(struct ifreq));
    strcpy(ifr_req.ifr_name, interface_name);

    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock_fd == -1)
    {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");

        return false;
    }

    if (ioctl(inet_sock_fd, SIOCGIFHWADDR, &ifr_req) == -1)
    {
        ALOGE("Failed to get flags for %s interface", interface_name);

        close(inet_sock_fd);
        return false;
    }

    if (ifr_req.ifr_hwaddr.sa_family!=ARPHRD_ETHER)
    {
        ALOGE("not an Ethernet interface");
    }

    const unsigned char* received_mac = (unsigned char*)ifr_req.ifr_hwaddr.sa_data;

    bool match = true;
    for (int pos = 0; pos <= 5; ++pos)
    {
        if (received_mac[pos] != mac_address[pos])
        {
            match = false;
            break;
        }
    }

    close(inet_sock_fd);
    return match;
}

static bool IsInterfaceUp(const char* interface_name)
{
    struct ifreq ifr_req;
    memset(&ifr_req, 0, sizeof(struct ifreq));
    strcpy(ifr_req.ifr_name, interface_name);

    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock_fd == -1)
    {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        return false;
    }

    if (ioctl(inet_sock_fd, SIOCGIFFLAGS, &ifr_req) == -1)
    {
        ALOGE("%s: Failed to get flags for interface", interface_name);
        close(inet_sock_fd);
        return false;
    }

    close(inet_sock_fd);
    return (ifr_req.ifr_flags & IFF_UP);
}

static bool BringInterfaceUp(const char* interface_name)
{
    struct ifreq ifr_req;
    memset(&ifr_req, 0, sizeof(struct ifreq));
    strcpy(ifr_req.ifr_name, interface_name);

    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock_fd == -1)
    {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        close(inet_sock_fd);
        return false;
    }

    if (ioctl(inet_sock_fd, SIOCGIFFLAGS, &ifr_req) == -1)
    {
        ALOGE("Failed to get flags for %s interface", interface_name);
        close(inet_sock_fd);
        return false;
    }

    ifr_req.ifr_flags |= (IFF_UP | IFF_RUNNING);

    if (ioctl(inet_sock_fd, SIOCSIFFLAGS, &ifr_req) == -1)
    {
        ALOGE("Failed to bring %s interface up", interface_name);
        close(inet_sock_fd);
        return false;
    }

    close(inet_sock_fd);

    return true;
}

static bool TakeInterfaceDown(const char* interface_name)
{
    struct ifreq ifr_req;
    memset(&ifr_req, 0, sizeof(struct ifreq));
    strcpy(ifr_req.ifr_name, interface_name);

    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock_fd == -1)
    {
        ALOGE("Failed to open AF_INET socket. Interface will not be configured");
        close(inet_sock_fd);
        return false;
    }

    if (ioctl(inet_sock_fd, SIOCGIFFLAGS, &ifr_req) == -1)
    {
        ALOGE("Failed to get flags for %s interface", interface_name);
        close(inet_sock_fd);
        return false;
    }

    ifr_req.ifr_flags &= ~(IFF_UP | IFF_RUNNING);

    if (ioctl(inet_sock_fd, SIOCSIFFLAGS, &ifr_req) == -1)
    {
        ALOGE("Failed to bring %s interface up", interface_name);
        close(inet_sock_fd);
        return false;
    }

    close(inet_sock_fd);

    return true;
}

bool SetNetmask(int skfd, const char *intf, const char *newmask)
{
    struct ifreq ifr;
    unsigned int dst;
    struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
    memset(&ifr, 0, sizeof(ifr));
    sin->sin_family = AF_INET;
    if ( !inet_pton(AF_INET, newmask, &sin->sin_addr) )
    {
        ALOGE("failed to convert netmask\n");
        return false;
    }
    strncpy( ifr.ifr_name, intf, IFNAMSIZ-1 );
    if ( ioctl(skfd, SIOCSIFNETMASK, &ifr) == -1 )
    {
        ALOGE("could not read interface %s\n", intf);
        return false;;
    }

    return true;
}

bool SetBroadcastAddress(int inet_sock_fd, const char *interface_name, const char *broadcast_address)
{
    struct sockaddr_in sin_addr;
    sin_addr.sin_family = AF_INET;
    sin_addr.sin_port = htons(0);

    // safe to ignore return value as ip address is a const string in correct format
    inet_aton(broadcast_address, (struct in_addr*)&sin_addr.sin_addr.s_addr);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    memcpy(&ifr.ifr_broadaddr, &sin_addr, sizeof(struct sockaddr));
    strcpy(ifr.ifr_name, interface_name);

    // Set broadcast address
    if (ioctl(inet_sock_fd, SIOCSIFBRDADDR, &ifr) == -1)
    {
        ALOGE("ioctl call set broadcast address failed. Error is [%s]", strerror(errno));
        return false;
    }

    return true;
}

static bool SetIpAddress(const char* interface_name, const char* ip_addr, const char* netmask, const char *broadcast_address)
{
    // Open AF_INET socket
    int inet_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock_fd == -1)
    {
        ALOGE("Failed to open AF_INET socket. Interface will not be cofigured");
        return false;
    }

    struct sockaddr_in sin_addr;
    sin_addr.sin_family = AF_INET;
    sin_addr.sin_port = htons(0);

    // safe to ignore return value as ip address is a const string in correct format
    inet_aton(ip_addr, (struct in_addr*)&sin_addr.sin_addr.s_addr);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    memcpy(&ifr.ifr_addr, &sin_addr, sizeof(struct sockaddr));
    strcpy(ifr.ifr_name, interface_name);

    // Set IP address
    if (ioctl(inet_sock_fd, SIOCSIFADDR, &ifr) == -1)
    {
        ALOGE("ioctl call set IP address failed. Error is [%s]", strerror(errno));
        close(inet_sock_fd);
        return false;
    }

    ALOGI("%s: Ip address set to %s", interface_name, ip_addr);

    if (!SetBroadcastAddress(inet_sock_fd, interface_name, broadcast_address))
    {
        ALOGE("ioctl call to set broadcast address failed. Error is [%s]", strerror(errno));
        close(inet_sock_fd);
        return false;
    }

    ALOGI("%s: Broadcast address set to %s", interface_name, broadcast_address);

    if (!SetNetmask(inet_sock_fd, interface_name, netmask))
    {
        ALOGE("ioctl call to set netmask failed. Error is [%s]", strerror(errno));
        close(inet_sock_fd);
        return false;
    }

    ALOGI("%s: Netmask set to %s", interface_name, netmask);

    close(inet_sock_fd);

    return true;
}

static bool SetMtu(const uint32_t mtu, const char* interface_name)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        ALOGE("Unable to open socket for set MTU address for device %s", interface_name);
        return false;
    }

    // Set MTU size for ethernet device
    struct ifreq ifr_mtu_size;

    strcpy(ifr_mtu_size.ifr_name, interface_name);
    ifr_mtu_size.ifr_addr.sa_family = AF_INET;
    ifr_mtu_size.ifr_mtu = mtu;
    if (ioctl(sockfd, SIOCSIFMTU, &ifr_mtu_size) == -1)
    {
        ALOGE("Unable to set MTU for device %s", interface_name);
        close(sockfd);
        return false;
    }
    else
    {
        ALOGI("%s: MTU set to %i", interface_name, mtu);
    }

    close(sockfd);

    return true;
}

static bool SetMacAddress(const std::vector<uint8_t> &mac_address, const char* interface_name)
{
    struct ifreq ifr_mac;
    int sockfd;

    for (int pos = 0; pos <= 5; ++pos)
    {
        ifr_mac.ifr_hwaddr.sa_data[pos] = mac_address[pos];
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        ALOGE("Unable to open socket for set MAC address for device. %s", interface_name);
        return false;
    }

    strcpy(ifr_mac.ifr_name, interface_name);
    ifr_mac.ifr_hwaddr.sa_family = ARPHRD_ETHER;

    //Set MAC address
    std::string printable_mac_address(mac_address.begin(), mac_address.end());
    if (ioctl(sockfd, SIOCSIFHWADDR, &ifr_mac) == -1)
    {
        close(sockfd);
        ALOGE("Unable to set MAC address (%s) for %s, errno %s.",
              printable_mac_address.c_str(),
              interface_name,
              strerror(errno));
        return false;
    }

    ALOGI("%s: MAC address set to %s", interface_name, printable_mac_address.c_str());

    close(sockfd);

    return true;
}

void SetupInterface(const std::vector<InterfaceConfiguration> &interface_configurations)
{
    for (auto &conf : interface_configurations) {
        SetupInterface(conf.name.c_str(),
                       conf.mac_address_bytes,
                       conf.ip_address.c_str(),
                       conf.netmask.c_str(),
                       conf.broadcast_address.c_str(),
                       conf.mtu);
    }
}

bool SetupInterface(const char* interface_name,
                    const std::vector<uint8_t> &mac_address,
                    const char* ip_addr,
                    const char* netmask,
                    const char* broadcast_addr,
                    const uint32_t mtu)
{
    ALOGI("%s: Setting configuration for network interface", interface_name);

    if (!InterfaceExists(interface_name))
    {
        ALOGE("%s: Interface does not appear to exist!", interface_name);
        return false;
    }

    if (!IsMacAddressCorrect(mac_address, interface_name))
    {
        if (IsInterfaceUp(interface_name))
        {
            TakeInterfaceDown(interface_name);
        }

        if (!SetMacAddress(mac_address, interface_name))
        {
            ALOGE("Failed to set MAC address for %s!", interface_name);
        }
    }
    else
    {
        ALOGI("%s: MAC address already set", interface_name);
    }

    // Set ip adress if ethernet interface is up already...
    if (!SetIpAddress(interface_name, ip_addr, netmask, broadcast_addr))
    {
        ALOGE("Failed to configure IP address for %s!", interface_name);
    }

    if (!SetMtu(mtu, interface_name))
    {
        ALOGE("Failed to set MTU for %s!", interface_name);
    }

    // All well so far; bring the interface up
    if (!IsInterfaceUp(interface_name))
    {
        // All well so far; bring the interface up
        if (!BringInterfaceUp(interface_name))
        {
            ALOGE("Failed to bring ethernet port up for %s!", interface_name);
        }
    }

    return true;
}

}
}
