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
#include "propertyhandler.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace ApplicationDataElement;

class CurveSpeedAdaptionModule {
  public:
    CurveSpeedAdaptionModule(vhal20::impl::IVehicleHalImpl*,
                             std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                             android::sp<SettingsFramework::SettingsManagerHidl> manager);

  private:
    VhalPropertyHandler<bool> prop_curve_speed_adaption_on;

    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_;
    DESender<autosar::AccAdprTurnSpdActv_info> accadpr_turnspd_actv_flexray_sender_;
    DEReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    DEReceiver<autosar::CrvtSpdWarnSts_info> crvtspdwarn_flexray_receiver_;
    DEReceiver<autosar::CrvtSpdAdpvSts_info> crvtspdadpv_flexray_receiver_;

    // "States"
    bool is_error_ = false;
    bool is_active_ = true;
    bool curve_speed_adaption_on_;
    SettingsFramework::ProfileIdentifier current_profile_ = SettingsFramework::ProfileIdentifier::None;

    // "Sub States"
    bool is_alivetimeout_ = false;
    bool is_invalid_crvtspdadpv_signal_ = false;

    void Update();
    void SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile);
};
