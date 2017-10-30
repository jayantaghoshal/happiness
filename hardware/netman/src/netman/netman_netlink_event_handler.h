/*
 * ===========================================================================
 * [2017]- Volvo Car Corporation
 * All Rights Reserved
 * LEGAL
 * NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
 * property of or licensed to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be
 * covered by patents or patent applications. This information is protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Volvo Car Corporation.
 */

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
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETMAN_NETLINK_EVENT_HANDLER_H_
