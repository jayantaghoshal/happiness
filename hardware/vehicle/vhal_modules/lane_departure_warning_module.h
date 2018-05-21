/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"
#include "papropertyhandler.h"

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "utils/vf_context.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace ApplicationDataElement;

class LaneDepartureWarningModule {
  public:
    LaneDepartureWarningModule(gsl::not_null<VFContext*> ctx);

  private:
    void Update();
    void SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile);
    PAPropHandler<bool> PA_prop_lane_departure_warning;

    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_;
    DESender<autosar::WarnTypForLaneChgWarn_info> warntyp_for_ldw_flexray_sender_;
    DESender<autosar::LaneChgWarnActv_info> ldw_actv_flexray_sender_;
    DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    DESynchronousReceiver<autosar::LaneChgWarnSts_info> lanechngwarnsts_flexray_receiver_;

    // "States"
    bool is_driving;
    bool is_alivetimeout_;
    bool ldw_service_req;
    bool is_active_;
    bool lane_departure_warning_on_;
    SettingsFramework::ProfileIdentifier current_profile_ = SettingsFramework::ProfileIdentifier::None;
};
