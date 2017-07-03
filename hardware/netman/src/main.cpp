#define  LOG_TAG    "netman_daemon"

#include <cutils/log.h>
#include <signal.h>
#include <stdlib.h>

#include <netman.h>

using namespace vcc::netman;

int socket_fd = 0;

void CleanUp()
{
    CloseNetlinkSocket(socket_fd);
    ALOGI("NetLink socket closed...");
}

void SigTermHandler(int sig)
{
    ALOGI("Terminate signal catched");
    CleanUp();
    exit(0);
}

void SigHupHandler(int sig)
{
    ALOGI("Sighup signal catched");
    CleanUp();
    exit(0);
}

int main()
{
    try
    {
        ALOGI("Netman started...");

        signal(SIGTERM, SigTermHandler);
        signal(SIGHUP, SigHupHandler);

        InterfaceConfiguration interface_conf;

        LoadInterfaceConfiguration(interface_conf);

        ALOGI("Configuration loaded...");
        ALOGI("Interface name: %s", interface_conf.name.c_str());
        ALOGI("Mac address: %s", interface_conf.mac_address.c_str());
        ALOGI("IP address: %s", interface_conf.ip_address.c_str());
        ALOGI("Netmask: %s", interface_conf.netmask.c_str());
        ALOGI("MTU: %i", interface_conf.mtu);

        if ((socket_fd = OpenNetlinkSocket()) == -1 )
        {
            ALOGE("Failed to open Netlink socket");
            return EXIT_FAILURE;
        }

        ALOGI("NetLink socket opened...");

        if (BindNetlinkSocket(socket_fd) == -1)
        {
            ALOGE("Failed to bind to Netlink socket");
            CloseNetlinkSocket(socket_fd);
            return EXIT_FAILURE;
        }

        ALOGI("NetLink socket bound...");

        ListenForNetlinkEvents(socket_fd, interface_conf);

        CloseNetlinkSocket(socket_fd);

        ALOGI("NetLink socket closed...");

        return EXIT_SUCCESS;

    }
    catch(const std::runtime_error &e)
    {

        ALOGE("ABORTING: Exception thrown: %s", e.what());

    }

    return EXIT_SUCCESS;
}
