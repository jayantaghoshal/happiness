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
class CTAWarning {
  public:
    CTAWarning();

  private:
    void InjectSignalsLeft();
    void InjectSignalsRight();
    ApplicationDataElement::DEReceiver<autosar::CtraIndcnLe_info> ctra_indcn_le;
    ApplicationDataElement::DEReceiver<autosar::CtraIndcnRi_info> ctra_indcn_ri;

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> veh_mod_mngt_glb_safe1;

    bool cta_active{false};

    SoundWrapper::SoundID play_sound_left =
            SoundWrapper::SoundID(AudioTable::SoundType::CrossTrafficAlert, AudioTable::SoundComponent::Left, false);
    SoundWrapper::SoundID play_sound_right =
            SoundWrapper::SoundID(AudioTable::SoundType::CrossTrafficAlert, AudioTable::SoundComponent::Right, false);
};
}
