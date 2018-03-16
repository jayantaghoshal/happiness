/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <Application_dataelement.h>
#include <ECD_dataelement.h>

namespace signal_proxy {

struct Proxies {
    autosar::HmiDefrstElecReq hmiDefrstElecReq_;
    autosar::HmiCmptmtTSp hmiCmptmtTSp_;
    autosar::HmiSeatClima seatClimateStatus_;
    autosar::HmiSeatClimaExtd seatClimateExtendStatus_;
    autosar::UsrSwtDispClimaReqForRowSec usrSwtDispClimaReqForRowSec_;
    autosar::UsrSwtDispClimaSts usrSwtDispClimaSts_;
};
}
