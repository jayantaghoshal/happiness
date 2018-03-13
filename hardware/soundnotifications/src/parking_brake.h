/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "Application_dataelement.h"
using namespace ApplicationDataElement;
using namespace autosar;

#include "soundwrapper.h"
namespace SoundNotifications {
namespace Chassis {
class ParkingBrake {
  public:
    ParkingBrake();
    void OnSignalChange();

  private:
    enum class EPBState {
        Idle,
        Active,
    };

    EPBState state;

    ApplicationDataElement::DEReceiver<autosar::VehSpdLgtSafe_info> VehSpdLgtSafe;
    ApplicationDataElement::DEReceiver<autosar::EpbLampReq_info> EpbLampReq;
};
}  // namespace Chassis
}  // namespace SoundNotifications
