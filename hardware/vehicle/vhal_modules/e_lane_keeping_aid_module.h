/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"

#include "modulepropertyhandler.h"
#include "notifiable_property.h"
#include "papropertyhandler.h"
#include "utils/vf_context.h"

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "utils/vf_context.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class ELaneKeepingAidModule {
  public:
    ELaneKeepingAidModule(gsl::not_null<VFContext*> ctx);

  private:
    PAPropHandler<bool> PA_prop_elane_keeping_aid_;

    // Settings
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_;

    // Sends flexray signals
    ApplicationDataElement::DESender<autosar::LaneKeepAidRoadEdgeActv_info> lanekeepingaid_flexray_sender_;

    // Receives flexray signals
    ApplicationDataElement::DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    ApplicationDataElement::DESynchronousReceiver<autosar::LaneKeepAidSts_info> lanekeepingaid_flexray_receiver_;

    // "States"
    bool is_error_;
    bool is_active_;
    bool is_usgMod_Drvg_;
    bool elane_keeping_aid_on_;
    SettingsFramework::ProfileIdentifier current_profile_;

    // "Sub States"
    bool is_eLKA_SrvRqrd_;
    bool is_alivetimeout_;
    bool is_invalid_signal_;

    bool is_set_to_Default_;

    void Update();
    void SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile);
};
