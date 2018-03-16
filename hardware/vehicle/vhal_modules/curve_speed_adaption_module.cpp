/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "curve_speed_adaption_module.h"
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "i_vehicle_hal_impl.h"
#include "interface_mapping.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

#undef LOG_TAG
#define LOG_TAG "CurveSpeedAdaption"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

vhal20::VehiclePropConfig propconfig_curve_speed_adaption_on() {
    auto prop = vccvhal10::VehicleProperty::CURVE_SPEED_ADAPTION_ON;
    return BoolConfig(prop);
}

CurveSpeedAdaptionModule::CurveSpeedAdaptionModule(vhal20::impl::IVehicleHalImpl* vehicleHal,
                                                   std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                                                   android::sp<SettingsFramework::SettingsManagerHidl> manager)
    : prop_curve_speed_adaption_on(propconfig_curve_speed_adaption_on(), dispatcher, vehicleHal),
      setting_(SettingId::CurveSpeedAdapt_On, true, manager) {
    // Check if enabled
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_148 = carconfig::getValue<CC148_CurveSpeedSupportType>();

    const bool is_enabled_ =
            (car_config_23 > CC23_CruiseControlType::Cruise_control &&
             car_config_148 > CC148_CurveSpeedSupportType::With_Curve_Speed_Warning_without_Curve_Speed_Assist);

    if (!is_enabled_) {
        ALOGV("Function disabled due to CarConfig.");
        curve_speed_adaption_on_ = false;
        SetFlexray(curve_speed_adaption_on_, current_profile_);
        return;
    }

    // Start listen to Settings
    setting_.setCallback([this](const auto& value) {
        ALOGV("Setting is changed: %d", value.value);
        curve_speed_adaption_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to Vhal
    prop_curve_speed_adaption_on.registerToVehicleHal();
    prop_curve_speed_adaption_on.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGV("VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        setting_.set(value);
    });

    // Check Alivetimout for error
    crvtspdwarn_flexray_receiver_.subscribe([&]() {
        const auto signal = crvtspdwarn_flexray_receiver_.get();
        is_alivetimeout_ = !signal.isOk();
        ALOGV("is_alivetimeout_ is changed: %d", is_alivetimeout_);
        Update();
    });

    // Check CrvtSpdAdpvSts for error
    crvtspdadpv_flexray_receiver_.subscribe([&]() {
        const auto signal = crvtspdadpv_flexray_receiver_.get();
        if (signal.isOk()) {
            auto crvtspdadpv_value = signal.value();
            is_invalid_crvtspdadpv_signal_ = (crvtspdadpv_value == autosar::OffOnNotAvlSrvRqrd::SrvRqrd);
            ALOGV("is_invalid_crvtspdadpv_signal_ is changed: %d", is_invalid_crvtspdadpv_signal_);
            Update();
        }
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModInActv ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModCnvinc ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg);

            ALOGV("is_active_ is changed: %d", is_active_);
            Update();
        }
    });
}

void CurveSpeedAdaptionModule::Update() {
    is_error_ = (is_alivetimeout_ || is_invalid_crvtspdadpv_signal_);

    if (is_error_) {
        SetFlexray(false, current_profile_);
        prop_curve_speed_adaption_on.PushProp(false);  // Set Vhal
    } else {
        if (!is_active_) {
            SetFlexray(false, current_profile_);
            prop_curve_speed_adaption_on.PushProp(false);  // Set Vhal
        } else {
            SetFlexray(curve_speed_adaption_on_, current_profile_);
            prop_curve_speed_adaption_on.PushProp(curve_speed_adaption_on_);  // Set Vhal
        }
    }
}

void CurveSpeedAdaptionModule::SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);

    OnOffPen flexray_value;
    flexray_value.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value.Pen = flexray_profile;
    accadpr_turnspd_actv_flexray_sender_.send(flexray_value);
}
