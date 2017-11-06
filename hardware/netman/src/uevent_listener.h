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

#ifndef _UEVENT_LISTENER_H_
#define _UEVENT_LISTENER_H_

#include "uevent_handler.h"

namespace vcc {
namespace netman {
class UeventHandler;

class NetlinkSocketListener final {
 public:
  static NetlinkSocketListener &Instance();
  ~NetlinkSocketListener();

  NetlinkSocketListener(NetlinkSocketListener &other) = delete;
  NetlinkSocketListener(NetlinkSocketListener &&other) = delete;

  NetlinkSocketListener &operator=(NetlinkSocketListener &other) = delete;
  NetlinkSocketListener &operator=(NetlinkSocketListener &&other) = delete;

  int StartListening();
  void StopListening();
  void SetNetlinkEventHandler(UeventHandler &event_handler);

 private:
  NetlinkSocketListener() = default;
  int SetupSocket();
  int RecvMessage();

  UeventHandler *event_handler_ = nullptr;
  int netlink_socket_ = -1;
};

}  // namespace netman
}  // namespace vcc

#endif  // _UEVENT_LISTENER_H_
