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

using namespace ApplicationDataElement;

class ConnectedSafetyModule {
  public:
    ConnectedSafetyModule(gsl::not_null<VFContext*> ctx);

  private:
    PAPropHandler<bool> PA_prop_connected_safety_on_;

    // Settings
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_connected_safety_on_;

    // FlexRay signals to send.
    DESender<autosar::RoadFricIndcnActv_info> roadfricindcnactv_flexray_sender_;
    DESender<ActivateVfc_info> activatevfc_;

    // FlexRay signals to receive.
    DESynchronousReceiver<autosar::RoadFricIndcnSts_info> roadfricindcnsts_flexray_receiver_;
    DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;

    // "States"
    bool is_error_;
    bool is_active_;
    bool connected_safety_on_;
    bool is_invalid_connected_safety_signal_;
    bool is_alivetimeout_;

    SettingsFramework::ProfileIdentifier current_profile_ = SettingsFramework::ProfileIdentifier::None;

    void Update();
    void SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile);
};
