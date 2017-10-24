#ifndef _NETMAN_NETLINK_EVENT_HANDLER_H_
#define _NETMAN_NETLINK_EVENT_HANDLER_H_

#include "netlink_event_handler.h"

namespace vcc {
namespace netman {

class NetmanNetlinkEventHandler final : public NetlinkEventHandler {
  const std::vector<InterfaceConfiguration> &interface_configurations_;

 public:
  NetmanNetlinkEventHandler(const std::vector<InterfaceConfiguration> &interface_configurations);

 protected:
  void HandleEvent(NetlinkEventData *eventData) override;

 private:
  void HandleNewLinkEvent(const struct ifinfomsg *if_info_msg);
  void HandleNewAddressEvent(const struct ifaddrmsg *if_addr_msg);
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETMAN_NETLINK_EVENT_HANDLER_H_
