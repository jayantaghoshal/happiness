/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef _UEVENT_LISTENER_H_
#define _UEVENT_LISTENER_H_

#include "memory"
#include "uevent_handler.h"

namespace vcc {
namespace netman {
class UeventHandler;

class UEventListener final {
  public:
    static UEventListener &Instance();
    ~UEventListener();

    UEventListener(const UEventListener &other) = delete;
    UEventListener(UEventListener &&other) = delete;

    UEventListener &operator=(const UEventListener &other) = delete;
    UEventListener &operator=(UEventListener &&other) = delete;

    int StartListening();
    void StopListening();
    void SetNetlinkEventHandler(UeventHandler &event_handler);

  private:
    UEventListener() = default;
    int SetupSocket();
    int RecvMessage();

    UeventHandler *event_handler_ = nullptr;
    int netlink_socket_ = -1;
};

}  // namespace netman
}  // namespace vcc

#endif  // _UEVENT_LISTENER_H_
