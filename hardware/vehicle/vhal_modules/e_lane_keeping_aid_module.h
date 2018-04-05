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
#include "i_vehicle_hal_impl.h"
#include "interface_mapping.h"
#include "modulepropertyhandler.h"
#include "notifiable_property.h"
#include "papropertyhandler.h"
#include "propertyhandler.h"
#include "vcc/localconfig.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace ApplicationDataElement;

class ELaneKeepingAidModule {
  public:
    ELaneKeepingAidModule(vhal20::impl::IVehicleHalImpl*,
                          std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                          android::sp<SettingsFramework::SettingsManagerHidl> manager);

  private:
    PAPropHandler<bool> PA_prop_elane_keeping_aid;

    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_;
    DESender<autosar::AccAdprTurnSpdActv_info> accadpr_turnspd_actv_flexray_sender_;
    DEReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    DEReceiver<autosar::CrvtSpdWarnSts_info> crvtspdwarn_flexray_receiver_;
    DEReceiver<autosar::CrvtSpdAdpvSts_info> crvtspdadpv_flexray_receiver_;

    // "States"
    bool is_error_ = false;
    bool is_active_ = true;
    bool elane_keeping_aid_on_;
    SettingsFramework::ProfileIdentifier current_profile_ = SettingsFramework::ProfileIdentifier::None;

    // "Sub States"
    bool is_alivetimeout_ = false;
    bool is_invalid_crvtspdadpv_signal_ = false;

    void Update();
    void SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile);
};
