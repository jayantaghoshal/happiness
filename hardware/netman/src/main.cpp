#define  LOG_TAG    "Netmand"

#include <cutils/log.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netlink_event_listener.h"
#include "netlink_event_handler.h"
#include "netman.h"

using namespace vcc::netman;

int main()
{
    try
    {
        ALOGI("Netmand 0.1 starting");

        ALOGI("Loading configuration from local config");

        std::vector<InterfaceConfiguration> interface_configurations;

        LoadInterfaceConfiguration(interface_configurations);

        ALOGI("Setting initial configuration on network interfaces");

        SetupInterface(interface_configurations);

        ALOGI("Initial configurations set");

        NetlinkEventHandler nl_event_handler(interface_configurations);

        NetlinkSocketListener &nl_socket_listener = NetlinkSocketListener::Instance();
        nl_socket_listener.SetNetlinkEventHandler(nl_event_handler);

        if (nl_socket_listener.StartListening()) {
            ALOGE("Unable to start NetlinkSocketListener (%s)", strerror(errno));
            exit(1);
        }

        exit(0);
    }
    catch(const std::runtime_error &e)
    {
        ALOGE("ABORTING: Exception thrown: %s", e.what());
    }

    exit(0);
}
