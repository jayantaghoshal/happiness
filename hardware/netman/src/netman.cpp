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

#include <netman.h>

namespace vcc {
namespace netman {

#define  LOG_TAG    "netman_daemon"

#define RECV_BUFFER_SIZE 4096

/* Declarations */

static bool RecvMessage(const int netlink_fd,
                        const InterfaceConfiguration &interface_conf);

static bool SetupInterface(const char* interface_name,
                           const std::vector<uint8_t> &mac_address,
                           const char* ip_addr,
                           const char* netmask,
                           const uint32_t mtu);

static void ConvertMacAddress(const std::string &mac_address,
                              std::vector<uint8_t> &mac_address_out);


/** Public Functions **/

void LoadInterfaceConfiguration(InterfaceConfiguration &conf)
{
    ALOGI("eth1.name %s", vcc::localconfig::GetString("eth1.name").c_str());
    conf.name = vcc::localconfig::GetString("eth1.name");
    conf.ip_address = vcc::localconfig::GetString("eth1.ip-address");
    conf.netmask = vcc::localconfig::GetString("eth1.netmask");
    conf.mac_address = vcc::localconfig::GetString("eth1.mac-address");
    conf.mtu = (uint32_t)vcc::localconfig::GetInt("eth1.mtu");
}

void ListenForNetlinkEvents(const int socket_fd, const InterfaceConfiguration &interface_conf)
{
    // We first try to set the interface if we have missed an earlier event
    std::vector<uint8_t> mac_address(5);
    ConvertMacAddress(interface_conf.mac_address, mac_address);

    ALOGI("Mac address converted...");

    SetupInterface(interface_conf.name.c_str(),
                   mac_address,
                   interface_conf.ip_address.c_str(),
                   interface_conf.netmask.c_str(),
                   interface_conf.mtu);

    ALOGI("Interface configuration set first time...");

    ALOGI("Entering event loop...");

    for (;;)
    {
        // recv_msg returns error only in fatal scenarios. In such a case; we should let application die
        if (!RecvMessage(socket_fd, interface_conf))
        {
            break;
        }
    }
}

int OpenNetlinkSocket()
{
    return socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
}

void CloseNetlinkSocket(const int socket_fd)
{
    close(socket_fd);
}

int BindNetlinkSocket(const int socket_fd)
{
     struct sockaddr_nl saddr = {.nl_family = AF_NETLINK, .nl_groups = RTMGRP_LINK};

     return bind(socket_fd, (struct sockaddr*)&saddr, sizeof(struct sockaddr_nl));
}

/** Private Functions */

static void ConvertMacAddress(const std::string &mac_address, std::vector<uint8_t> &mac_address_out)
{
    int byte;

    char skip_byte;

    ALOGI("Init conversion of mac address...");

    std::stringstream address(mac_address, std::ios_base::in);

    address >> std::hex;

    ALOGI("Converting mac address...");
    for (int pos = 0; pos <= 5; ++pos)
    {
        address >> byte >> skip_byte;

        mac_address_out[pos] = byte;
    }
}

static bool RecvMessage(const int netlink_fd, const InterfaceConfiguration &interface_conf)
{
    const char *interface_name = interface_conf.name.c_str();
    const char *ip_addr = interface_conf.ip_address.c_str();
    const char *netmask = interface_conf.netmask.c_str();
    const int mtu = interface_conf.mtu;
    std::vector<uint8_t> mac_address(5);

    ConvertMacAddress(interface_conf.mac_address, mac_address);

    char buf[RECV_BUFFER_SIZE] = {0};
    struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
    struct sockaddr_nl sa;
    struct msghdr msg = {.msg_name = (void*)&sa,
                         .msg_namelen = sizeof(sa),
                         .msg_iov = &iov,
                         .msg_iovlen = 1,
                         .msg_control = NULL,
                         .msg_controllen = 0,
                         .msg_flags = 0};

    struct nlmsghdr* nl_message_header;
    int len;
    while (true)
    {
        len = recvmsg(netlink_fd, &msg, 0);
        if (len != -1)
            break;

        if (errno != EINTR)
        {
            ALOGE("recvmsg system call reported error is [%s]. Program will terminate", strerror(errno));
            return false;
        }
    }

    if (len == 0)
    {
        // socket got disconnected?
        ALOGE("NETLINK socket got disconnected. Program will terminate");
        return false;
    }

    for (nl_message_header = (struct nlmsghdr*)buf;
         NLMSG_OK(nl_message_header, len);
         nl_message_header = NLMSG_NEXT(nl_message_header, len))
    {
        if (nl_message_header->nlmsg_type == NLMSG_DONE)
        {
            return true;
        }

        if (nl_message_header->nlmsg_type == NLMSG_ERROR)
        {
            continue;
        }

        if (nl_message_header->nlmsg_type == RTM_NEWLINK)
        {
            struct ifinfomsg* ifimsg = (struct ifinfomsg*)NLMSG_DATA(nl_message_header);
            char name[IF_NAMESIZE];

            ALOGI("RTM_NEWLINK received...(ifi_flags = 0x%x)", ifimsg->ifi_flags);

            if ((ifimsg != NULL) &&
                (if_indextoname(ifimsg->ifi_index, name) != NULL) &&
                (ifimsg->ifi_flags & IFF_UP) &&
                !(ifimsg->ifi_flags & IFF_RUNNING))
            {
                ALOGI("RTM_NEWLINK Link UP for interface %s...", name);
                if (0 == strncmp(name, interface_name, strlen(interface_name)))
                {
                    SetupInterface(interface_name, mac_address, ip_addr, netmask, mtu);
                }
                else
                {
                    ALOGI("RTM_NEWLINK Link interface %s does not match expected %s", name, interface_name);
                }
                // received NEWLINK event is not for meth0
            }
        }
        // else ignore uninterested events
    }

    return true;
}

// Constants

// Helper functions
static bool InterfaceExists(const char* interface_name)
{
    //TODO: Find a better way to check if interface exists... Checking if we can retreive IFF flags maybe isn't the best way?!
    ALOGI("Entering InterfaceExists...");

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

    close(inet_sock_fd);
    return true;
}

static bool IsMacAddressCorrect(const std::vector<uint8_t> &mac_address, const char* interface_name)
{
    ALOGI("Entering IsMacAddressCorrect...");

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

    ALOGI("Comparing MAC adress expected MAC (%02X:%02X:%02X:%02X:%02X:%02X) with received MAC (%02X:%02X:%02X:%02X:%02X:%02X)",
          mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5],
          received_mac[0], received_mac[1], received_mac[2], received_mac[3], received_mac[4], received_mac[5]);

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
    ALOGI("Entering IsInterfaceUp...");

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
        ALOGE("Failed to get flags for %s interface", interface_name);
        close(inet_sock_fd);
        return false;
    }

    close(inet_sock_fd);
    return (ifr_req.ifr_flags & IFF_UP);
}

static bool BringInterfaceUp(const char* interface_name)
{
    ALOGI("Entering BringInterfaceUp...");

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
    ALOGI("Entering TakeInterfaceDown...");

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

static bool SetIpAddress(const char* interface_name, const char* ip_addr, const char* netmask)
{
    ALOGI("Entering SetIpAddress...");

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


    ALOGI("Setting IP address...");
    // Set IP address
    if (ioctl(inet_sock_fd, SIOCSIFADDR, &ifr) == -1)
    {
        ALOGE("ioctl call set IP address failed. Error is [%s]", strerror(errno));
        close(inet_sock_fd);
        return false;
    }

    ALOGI("Setting Netmask...");
    // Set Netmask
    // safe to ignore return value as netmask is constant string in correct format
    inet_aton(netmask, (struct in_addr*)&sin_addr.sin_addr.s_addr);
    memset(&ifr, 0, sizeof(struct ifreq));
    memcpy(&ifr.ifr_netmask, &sin_addr, sizeof(struct sockaddr));
    strcpy(ifr.ifr_name, interface_name);

    if (ioctl(inet_sock_fd, SIOCSIFNETMASK, &ifr) == -1)
    {
        ALOGE("ioctl call to set netmask failed. Error is [%s]", strerror(errno));
        close(inet_sock_fd);
        return false;
    }

    close(inet_sock_fd);

    ALOGI("Exiting SetIpAddress...");

    return true;
}

static bool SetMtu(const uint32_t mtu, const char* interface_name)
{
    ALOGI("Entering SetMtu...");

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
        ALOGI("Successfully set MTU for device %s", interface_name);
    }

    close(sockfd);

    return true;
}

static bool SetMacAddress(const std::vector<uint8_t> &mac_address, const char* interface_name)
{
    ALOGI("Entering SetMacAddress...");

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
    if (ioctl(sockfd, SIOCSIFHWADDR, &ifr_mac) == -1)
    {
        close(sockfd);
        std::string printable_mac_address(mac_address.begin(), mac_address.end());
        ALOGE("Unable to set MAC address (%s) for %s, errno %s.",
              printable_mac_address.c_str(),
              interface_name,
              strerror(errno));
        return false;
    }
    else
    {
        ALOGI("Successfully set MAC address for device %s", interface_name);
    }

    close(sockfd);

    return true;
}

static bool SetupInterface(const char* interface_name,
                           const std::vector<uint8_t> &mac_address,
                           const char* ip_addr,
                           const char* netmask,
                           const uint32_t mtu)
{
    ALOGI("Entering SetupInterface...");

    //First check if interface exists
    if (!InterfaceExists(interface_name))
    {
        ALOGE("Interface %s does not appear to exist!", interface_name);
        return false;
    }

    if (IsMacAddressCorrect(mac_address, interface_name))
    {
        ALOGI("MAC address already set for %s!", interface_name);
    }
    else
    {
        if (IsInterfaceUp(interface_name))
        {
            ALOGI("Interface %s is UP, taking it down...", interface_name);

            TakeInterfaceDown(interface_name);
        }
        else
        {
            ALOGI("Interface %s is already DOWN!", interface_name);
        }

        if (!SetMacAddress(mac_address, interface_name))
        {
            ALOGE("Failed to set MAC address for %s!", interface_name);
        }
    }

    // Set ip adress if ethernet interface is up already...
    if (!SetIpAddress(interface_name, ip_addr, netmask))
    {
        ALOGE("Failed to configure IP address for %s!", interface_name);
    }

    // All well so far; bring the interface up
    if (!SetMtu(mtu, interface_name))
    {
        ALOGE("Failed to set MTU for %s!", interface_name);
    }

    if (!IsInterfaceUp(interface_name))
    {
        ALOGI("Calling BringInterfaceUp...");
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
