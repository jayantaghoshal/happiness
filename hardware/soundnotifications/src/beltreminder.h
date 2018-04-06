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
class BeltReminder {
  public:
    BeltReminder();

  private:
    template <typename S>
    void injectSignals(S bltSnd);

    ApplicationDataElement::DEReceiver<autosar::BltRmnSound1_info> bltSnd1Receiver;
    ApplicationDataElement::DEReceiver<autosar::BltRmnSound2_info> bltSnd2Receiver;
    ApplicationDataElement::DEReceiver<autosar::BltRmnSound3_info> bltSnd3Receiver;
    ApplicationDataElement::DEReceiver<autosar::BltRmnSound4_info> bltSnd4Receiver;

    SoundWrapper::SoundID lastPlayedSound;
};
}
