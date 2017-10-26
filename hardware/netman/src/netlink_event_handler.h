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

#ifndef _NETLINK_EVENT_HANDLER_H_
#define _NETLINK_EVENT_HANDLER_H_

#include "interface_configuration.h"
#include "netlink_event_listener.h"

namespace vcc {
namespace netman {

class NetlinkEventHandler {
 public:
  enum class NetlinkEventType { NETLINK_NEW_LINK, NETLINK_NEW_ADDRESS, NETLINK_UEVENT };

  void HandleEvent(struct nlmsghdr* nl_message_header);
  void HandleEvent(char* message, const int length);

 protected:
  struct NetlinkEventData {
    NetlinkEventData(NetlinkEventType evtType) : eventType(evtType) {}
    virtual ~NetlinkEventData() = default;
    NetlinkEventType eventType;
  };

  struct NetlinkNewLinkEvent : NetlinkEventData {
    NetlinkNewLinkEvent(struct ifinfomsg* msg) : NetlinkEventData(NetlinkEventType::NETLINK_NEW_LINK), info_msg(msg) {}
    const struct ifinfomsg* info_msg;
  };

  struct NetlinkNewAddrEvent : NetlinkEventData {
    NetlinkNewAddrEvent(struct ifaddrmsg* msg)
        : NetlinkEventData(NetlinkEventType::NETLINK_NEW_ADDRESS), addr_msg(msg) {}
    const struct ifaddrmsg* addr_msg;
  };

  struct NetlinkUevent : NetlinkEventData {
    NetlinkUevent(char* msg, int len) : NetlinkEventData(NetlinkEventType::NETLINK_UEVENT), uevent(msg), length(len) {}
    const char* uevent;
    const int length;
  };

  virtual void HandleEvent(NetlinkEventData* eventData) = 0;
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETLINK_EVENT_HANDLER_H_
