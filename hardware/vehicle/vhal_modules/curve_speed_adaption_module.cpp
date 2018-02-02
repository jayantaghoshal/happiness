/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include <functional>
#include <memory>
#include "carconfig.h"
#include "cc_parameterlist.h"

#include "curve_speed_adaption_module.h"
#include "i_vehicle_hal_impl.h"
#include "interface_mapping.h"

#undef LOG_TAG
#define LOG_TAG "CurveSpeedAdaption"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

CurveSpeedAdaptionModule::CurveSpeedAdaptionModule(
        ::android::hardware::automotive::vehicle::V2_0::impl::IVehicleHalImpl* vehicleHal,
        android::sp<SettingsFramework::SettingsManagerHidl> manager)
    : ModulePropertyHandler(vehicleHal), setting_(SettingId::CurveSpeedAdapt_On, true, manager) {
    // Handle if function isnt avaliable due to CarConfig conditions isnt fullfilled.
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_148 = carconfig::getValue<CC148_CurveSpeedSupportType>();

    bool car_config_invalid =
            !(car_config_23 > CC23_CruiseControlType::Cruise_control &&
              car_config_148 > CC148_CurveSpeedSupportType::With_Curve_Speed_Warning_without_Curve_Speed_Assist);
    if (car_config_invalid) {
        // TODO, waiting for confirmation from Rasmus if hardcoded None is ok to set.
        ALOGV("Function disabled due to CarConfig.");
        curve_speed_adaption_on_ = false;
        SetFlexray(curve_speed_adaption_on_, current_profile_);
        return;
    }

    // Check Alivetimout if function is in error-state or not
    crvtspdwarn_flexray_receiver_.subscribe([&]() {
        const auto signal = crvtspdwarn_flexray_receiver_.get();
        is_alivetimeout_ = !signal.isOk();
        CheckSystemError();
    });

    // Check CrvtSpdAdpvSts for valid signals during active if function is in error-state or not
    crvtspdadpv_flexray_receiver_.subscribe([&]() {
        const auto signal = crvtspdadpv_flexray_receiver_.get();
        if (signal.isOk()) {
            auto crvtspdadpv_value = signal.value();

            is_invalid_crvtspdadpv_signal_ = (crvtspdadpv_value == autosar::OffOnNotAvlSrvRqrd::SrvRqrd);
            CheckSystemError();
        }
    });

    // Check UsageMod if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();
            ALOGV("Update from flexray, VehModMngtGlbSafe1: value:%d",
                  static_cast<autosar::UsgModSts1>(vehmod_value.UsgModSts));

            function_is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModInActv ||
                                   vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModCnvinc ||
                                   vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg);

            if (!is_error_) {
                if (!function_is_active_) {
                    SetFlexray(false, current_profile_);
                    SetVhal(false);
                } else {
                    SetFlexray(curve_speed_adaption_on_, current_profile_);
                    SetVhal(curve_speed_adaption_on_);
                }
            }
        }
    });

    // Settings
    setting_.setCallback([this](const auto& value) {
        ALOGV("Update from Setting: value=%d, profileId=%d", value.value, value.profileId);
        curve_speed_adaption_on_ = value.value;
        current_profile_ = value.profileId;

        if (!is_error_ && function_is_active_) {
            SetFlexray(curve_speed_adaption_on_, current_profile_);
            SetVhal(curve_speed_adaption_on_);
        }
    });

    // Config setup
    PropertyAndConfig property;
    property.prop_config.prop = toInt(vccvhal10::VehicleProperty::CURVE_SPEED_ADAPTION_ON);
    property.prop_config.access = VehiclePropertyAccess::READ_WRITE;
    property.prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;

    // Value setup
    vhal20::VehiclePropValue prop_value;
    prop_value.prop = property.prop_config.prop;
    prop_value.areaId = 0;  // GLOBAL

    property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
            std::make_unique<PropertyHandlerInt32>(
                    [this](int32_t vhal_value) {
                        ALOGV("Update from vhal: value=%d", vhal_value);

                        SetSetting(curve_speed_adaption_on_);  // Set Setting will also update autosar +
                                                               // curve_speed_adaption_on_
                        return 0;
                    },
                    [this]() { return &vhal_value_; },
                    [this](vhal20::VehiclePropValue prop_value) { pushProp(prop_value); }, prop_value);

    // Property handler configured, put it in map
    propertyhandlers_[static_cast<vccvhal10::VehicleProperty>(prop_value.prop)] = std::move(property);
}

void CurveSpeedAdaptionModule::CheckSystemError() {
    ALOGV("CheckSystemError: is_alivetimeout_=%d, is_invalid_crvtspdadpv_signal_=%d", is_alivetimeout_,
          is_invalid_crvtspdadpv_signal_);
    is_error_ = (is_alivetimeout_ || is_invalid_crvtspdadpv_signal_);

    if (is_error_) {
        SetFlexray(false, current_profile_);
        SetVhal(false);
    } else {
        if (!function_is_active_) {
            SetFlexray(false, current_profile_);
            SetVhal(false);
        } else {
            SetFlexray(curve_speed_adaption_on_, current_profile_);
            SetVhal(curve_speed_adaption_on_);
        }
    }
}

void CurveSpeedAdaptionModule::SetSetting(bool value) {
    ALOGV("SetSetting: value=%d", value);
    setting_.setForProfile(value);  // Set Setting will also update autosar
}

void CurveSpeedAdaptionModule::SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);
    ALOGV("SetFlexray: value=%d, profile=%d", value, flexray_profile);

    // To flexray
    OnOffPen flexray_value;
    flexray_value.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value.Pen = flexray_profile;
    accadpr_turnspd_actv_flexray_sender_.send(flexray_value);
}

void CurveSpeedAdaptionModule::SetVhal(bool value) {
    // To vhal (bool sent as int)
    vhal_value_.set(static_cast<int32_t>(value));
}