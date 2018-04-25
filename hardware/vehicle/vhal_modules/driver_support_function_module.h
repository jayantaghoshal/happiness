/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"

#include "carconfig.h"
#include "cc_parameterlist.h"
#include "modulepropertyhandler.h"
#include "notifiable_property.h"
#include "papropertyhandler.h"
#include "propertyhandler.h"

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "utils/vf_context.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class DriverSupportFunctionModule {
  public:
    DriverSupportFunctionModule(gsl::not_null<VFContext*> ctx);

  private:
    PAPropHandler<int> pa_driver_support_function_on;

    SettingsFramework::Setting<autosar::DrvrSpprtFct, SettingsFramework::UserScope::USER> setting_;
    ApplicationDataElement::DESender<autosar::UsrSeldDrvrSpprtFct_info> usrseld_drvr_SpprtFct_flexray_sender_;
    ApplicationDataElement::DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    ApplicationDataElement::DESynchronousReceiver<autosar::CrsCtrlrSts_info> crsCtrlrSts_flexray_receiver_;
    ApplicationDataElement::DESynchronousReceiver<autosar::AccSts_info> accadpr_flexray_receiver_;
    ApplicationDataElement::DESynchronousReceiver<autosar::AdjSpdLimnSts_info> adj_spd_limn_sts_receiver_;

    bool vhal_acc_enabled;
    bool vhal_cc_enabled;
    bool vhal_sl_enabled;

    bool acc_sts_off;
    bool cc_sts_off;
    bool sl_sts_off;

    // "States"
    bool is_active_;

    autosar::DrvrSpprtFct drvr_spprt_func_on_;
    vccvhal10::PAStatus drvr_spprt_func_status;

    void Update();
    void SetFlexray(autosar::DrvrSpprtFct value);
    void SetVhal(autosar::DrvrSpprtFct value, vccvhal10::PAStatus status);
};
