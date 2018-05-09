/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "Application_dataelement.h"
using namespace ApplicationDataElement;
using namespace autosar;

#include "soundwrapper.h"
namespace SoundNotifications {
class ForwardCollision {
  public:
    ForwardCollision();
    void OnSignalChange_CllsnWarnReq();
    void OnSignalChange_CllsnFwdWarnReq();

  private:
    SoundWrapper::SoundID CllsnWarnSoundId;
    ApplicationDataElement::DEReceiver<autosar::CllsnWarnReq_info> CllsnWarnReq;
    ApplicationDataElement::DEReceiver<autosar::CllsnFwdWarnReq_info> CllsnFwdWarnReq;
};
}  // namespace SoundNotifications
