/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"

using namespace ApplicationDataElement;

class SpeedLimitAdaptationModule {
  public:
    SpeedLimitAdaptationModule();

    // Receiver port
    DEReceiver<autosar::DrvrHmiSpdLimAdpnSts_info> drvrhmi_spdlimadpn_sts_flexray_receiver_;

    // Sender port
    DESender<autosar::AccAdprSpdLimActv_info> accadpr_spdlim_actv_flexray_sender_;

    // "State"
    bool speed_limit_adaptation_on = false;

    // Update function
    void Update();
};
