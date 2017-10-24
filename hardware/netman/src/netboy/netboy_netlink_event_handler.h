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
