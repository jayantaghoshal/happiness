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

        InterfaceConfiguration eth1_configuration;

        LoadInterfaceConfiguration(eth1_configuration);

        SetupInterface(eth1_configuration.name.c_str(),
                       eth1_configuration.mac_address_bytes,
                       eth1_configuration.ip_address.c_str(),
                       eth1_configuration.netmask.c_str(),
                       eth1_configuration.broadcast_address.c_str(),
                       eth1_configuration.mtu);

        ALOGI("Initial eth1 configuration set");

        NetlinkEventHandler nl_event_handler(eth1_configuration);

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
