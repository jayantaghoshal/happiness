/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "dispatcher.h"
#include "vfc_helper.h"

#include <ECD_dataelement.h>

class VFCDrivingClimatisationLogic
{

public:
    explicit VFCDrivingClimatisationLogic(IDispatcher& timerDispatcher);

private:
    void handleSignals();

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1_;
    ECDDataElement::DESink<autosar::HmiHvacFanLvlFrnt_info>              levelOfFanLevelFront_;
    ECDDataElement::DESink<autosar::HmiSeatClima_info>                   seatHeatAndVent_;
    ECDDataElement::DESink<autosar::HmiSeatClimaExtd_info>               seatVentEx_;

    IDispatcher& timerDispatcher_;
    bool         activate_;
};
