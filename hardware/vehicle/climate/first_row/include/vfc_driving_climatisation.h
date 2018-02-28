/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "legacydispatcher.h"
#include "vfc_helper.h"

#include <ECD_dataelement.h>

class VFCDrivingClimatisationLogic {
  public:
    explicit VFCDrivingClimatisationLogic(ILegacyDispatcher& timerDispatcher);

  private:
    void handleSignals();

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1_;
    ECDDataElement::DESink<autosar::HmiHvacFanLvlFrnt_info> levelOfFanLevelFront_;
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatAndVent_;
    ECDDataElement::DESink<autosar::HmiSeatClimaExtd_info> seatVentEx_;

    ILegacyDispatcher& timerDispatcher_;
    bool activate_;
};
