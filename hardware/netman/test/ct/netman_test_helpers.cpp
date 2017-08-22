#include "netman_test_helpers.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

std::string GetIpAddressForInterface(const std::string& ifname) {
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ-1);

    int r = ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    if (r==0) {
        return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    } else {
        return "";
    }
}

std::string GetNetmaskForInterface(const std::string& ifname)
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ-1);

    int r = ioctl(fd, SIOCGIFNETMASK, &ifr);

    close(fd);

    if (r==0) {
        return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr);
    } else {
        return "";
    }
}

std::string GetBroadcastAddressForInterface(const std::string &ifname)
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ-1);

    int r = ioctl(fd, SIOCGIFBRDADDR, &ifr);

    close(fd);

    if (r==0) {
        return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr);
    } else {
        return "";
    }
}

int GetMtuForInterface(const std::string& ifname)
{
    int fd;
    struct ifreq ifr;
    unsigned char *mac;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , ifname.c_str() , IFNAMSIZ-1);

    int r = ioctl(fd, SIOCGIFMTU, &ifr);

    close(fd);

    if (r==0) {
        return ifr.ifr_mtu;
    } else {
        return -1;
    }
}

std::string GetMacAddressForInterface(const std::string& ifname)
{
    int fd;
    struct ifreq ifr;
    unsigned char *mac;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , ifname.c_str() , IFNAMSIZ-1);

    int r = ioctl(fd, SIOCGIFHWADDR, &ifr);

    close(fd);

    if (r != 0)
        return "";

    mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;

    const int MAC_ADDRESS_LENGTH = 6;
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < MAC_ADDRESS_LENGTH; i++) {
        ss <<  std::setw(2) << (int) mac[i];
        if (i < MAC_ADDRESS_LENGTH - 1)
            ss << ':';
    }

    return ss.str();
}

pid_t FindProcessId(const std::string& name)
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL;

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL) {
        long lpid = atol(ent->d_name);
        if(lpid < 0)
            continue;
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if (fp) {
            if ( (fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3 ){
                printf("fscanf failed \n");
                fclose(fp);
                closedir(dir);
                return -1;
            }
            if (pname == name) {
                fclose(fp);
                closedir(dir);
                return (pid_t)lpid;
            }
            fclose(fp);
        }
    }

    closedir(dir);
    return -1;
}
