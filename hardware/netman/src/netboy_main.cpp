#include <cutils/log.h>
#include <cutils/properties.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netlink_event_listener.h"
#include "netboy_netlink_event_handler.h"
#include "netman.h"

#define  LOG_TAG    "Netboyd"

using namespace vcc::netman;

int main()
{
    try
    {
        ALOGI("Net Boy 0.1 starting");

        ALOGI("Moving initial network interfaces");

        VccNamespaceInit();

        NetboyNetlinkEventHandler nl_event_handler;

        NetlinkSocketListener &nl_socket_listener = NetlinkSocketListener::Instance();
        nl_socket_listener.SetNetlinkEventHandler(nl_event_handler);

        property_set("netboyd.startup_completed", "1");

        if (nl_socket_listener.StartListening()) {
            ALOGE("Unable to start NetlinkSocketListener (%s)", strerror(errno));
            exit(1);
        }
    }
    catch(const std::runtime_error &e)
    {
        ALOGE("ABORTING: Exception thrown: %s", e.what());
        exit(1);
    }

    return 0;
}
