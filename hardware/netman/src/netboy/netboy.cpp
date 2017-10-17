#include <errno.h>

#include <cutils/log.h>
#include <cutils/properties.h>

#include "netboy_netlink_event_handler.h"
#include "netlink_event_listener.h"
#include "rule_handler.h"
#include "vcc/localconfig.h"

#define LOG_TAG "Netboyd"

using namespace vcc::netman;

int main() {
  try {
    ALOGI("Net Boy 0.1 starting");

    auto *lcfg = vcc::LocalConfigDefault();

    RuleHandler rule_handler;
    if (!rule_handler.loadRules(lcfg)) {
      ALOGE("Unable to read rules from local config");
      return 1;
    }

    NetboyNetlinkEventHandler::SysfsNetSubsystemWalker();

    NetboyNetlinkEventHandler nl_event_handler;

    NetlinkSocketListener &nl_socket_listener =
        NetlinkSocketListener::Instance(NetlinkSocketListener::SocketType::NLSOC_TYPE_UEVENT);

    nl_socket_listener.SetNetlinkEventHandler(nl_event_handler);

    // Need to set property before Blocking on netlink socket
    property_set("netboyd.startup_completed", "1");

    if (nl_socket_listener.StartListening() < 0) {
      ALOGE("Unable to start NetlinkSocketListener (%s)", strerror(errno));
      return EXIT_FAILURE;
    }
  } catch (const std::runtime_error &e) {
    ALOGE("ABORTING: Exception thrown: %s", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
