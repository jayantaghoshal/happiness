/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "Application_dataelement.h"
#include "soundwrapper.h"
using namespace ApplicationDataElement;
using namespace autosar;

namespace SoundNotifications {
class ACCBrakeReleaseWarning {
  public:
    ACCBrakeReleaseWarning();

  private:
    void InjectSignals();
    ApplicationDataElement::DEReceiver<autosar::BrkRelsWarnReq_info> BrkRelsWarnReq;

    SoundWrapper::SoundID lastPlayedSound;
};
}
