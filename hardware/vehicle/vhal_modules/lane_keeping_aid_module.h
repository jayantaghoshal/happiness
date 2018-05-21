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
#include "propertyhandler.h"

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "utils/vf_context.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

using namespace ApplicationDataElement;

class LaneKeepingAidModule {
  public:
    LaneKeepingAidModule(gsl::not_null<VFContext*> ctx);

  private:
    PAPropHandler<bool> PA_prop_lane_keeping_aid_on_;
    PAPropHandler<int> PA_prop_lane_keeping_aid_mode_;

    // Settings
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_lka_on_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> setting_lka_mode_;
    // Send FlexRay signal
    DESender<autosar::LaneKeepAidActv_info> lanekeepaidactv_flexray_sender_;
    DESender<autosar::IntvAndWarnModForLaneKeepAid_info> intv_and_warnmod_for_lanekeepaid_flexray_sender_;
    DESender<autosar::WarnTypForLaneKeepAid_info> warntypforlanekeepaid_flexray_sender_;
    DESender<ActivateVfc_info> activatevfc_;
    // Receive FlexRay signal
    DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    DESynchronousReceiver<autosar::LaneKeepAidSts_info> lane_keep_aid_sts_receiver_;

    // "States"
    bool is_error_;
    bool is_active_;
    bool lane_keeping_aid_on_;
    int lane_keeping_aid_mode_;
    bool is_invalid_lane_keep_aid_sts_signal_;
    bool is_alivetimeout_;

    SettingsFramework::ProfileIdentifier current_profile_ = SettingsFramework::ProfileIdentifier::None;

    void Update();
    void SetFlexray(bool value, int mode, SettingsFramework::ProfileIdentifier current_profile);
    bool isModeInRange(int mode);
};
