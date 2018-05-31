/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"

#include "papropertyhandler.h"

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "utils/vf_context.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace ApplicationDataElement;

class ParkingModule {
  public:
    ParkingModule(gsl::not_null<VFContext*> ctx);

  private:
    PAPropHandler<bool> PA_prop_request_pas_hide;

    // Receivers
    DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    DESynchronousReceiver<autosar::TrlrPrsntSts1WdSts_info> trlrprsnt_flexray_receiver_;
    DESynchronousReceiver<autosar::PrkgAssiActvSts_info> prkgassiactvsts_flexray_receiver_;
    DESynchronousReceiver<autosar::NFSDataFront_info> nfsdatafront_flexray_receiver_;
    DESynchronousReceiver<autosar::NFSDataRear_info> nfsdatarear_flexray_receiver_;
    DESynchronousReceiver<autosar::AutnmsPrkgActvScenario_info> autnmsprkgactvscenario_flexray_receiver_;

    // "States"
    bool is_error_;
    bool is_active_;
    bool request_pas_hide_;
    bool is_invalid_pas_signal_;
    bool is_alivetimeout_;

    void Update();
};
