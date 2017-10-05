#pragma once

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include "vendor/volvocars/hardware/signals/1.0/ISignals.h"
#include "vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h"
#include "vendor/volvocars/hardware/signals/1.0/types.h"
using namespace vendor::volvocars::hardware::signals::V1_0;

#include "socket_connection.h"

namespace CarSim {

class HidlHandler : public ISignalsChangedCallback {
 public:
  // The callbackfunction for HIDL
  ::android::hardware::Return<void> signalChanged(const ::android::hardware::hidl_string& signalName, Dir dir,
                                                  const ::android::hardware::hidl_string& data) override;

  // The handle to client connection.
  void SetSocketConnection(std::shared_ptr<SocketConnection> connection);

 private:
  std::shared_ptr<CarSim::SocketConnection> connection_;
};

}  // CarSim