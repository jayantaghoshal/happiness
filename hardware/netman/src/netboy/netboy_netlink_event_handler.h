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

#ifndef _NETBOY_NETLINK_EVENT_HANDLER_H_
#define _NETBOY_NETLINK_EVENT_HANDLER_H_

#include "netlink_event_handler.h"

namespace vcc {
namespace netman {

class NetboyNetlinkEventHandler final : public NetlinkEventHandler {
 public:
  static int SysfsNetSubsystemWalker();

 protected:
  void HandleEvent(NetlinkEventData* eventData) override;

 private:
  void HandleUevent(const char* uevent, const int message_length);
  static int HandleSysfsEntry(const char* filepath, const struct stat* info, const int typeflag, struct FTW* pathinfo);
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETBOY_NETLINK_EVENT_HANDLER_H_
