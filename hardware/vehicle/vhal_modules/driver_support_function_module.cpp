/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "driver_support_function_module.h"
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include "carconfig.h"
#include "i_vehicle_hal_impl.h"
#include "interface_mapping.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

#undef LOG_TAG
#define LOG_TAG "DriverSupportFunction"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

enum DriverSupportFunc { CC, ACC, SL };

vhal20::VehiclePropConfig propconfig_driver_support_function_on() {
    auto prop = vccvhal10::VehicleProperty::DRIVER_SUPPORT_FUNCTION_ON;

    // Check carconfig
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_36 = carconfig::getValue<CC36_AdjustableSpeedLimiterType>();

    /*
    TODO: Use configFlags instead of maxValue.
    Configuration bits for Cruise Control modes.
    Bit 2 = CC  = 4
    Bit 3 = ACC = 8
    Bit 4 = SL  = 16
    */
    int32_t maxValue = 0b00000011;  // in case of validation, init to value > 3

    if (car_config_23 > CC23_CruiseControlType::No_Cruise_control) {
        maxValue |= 0b00000100;
    }
    if (car_config_23 > CC23_CruiseControlType::Cruise_control) {
        maxValue |= 0b00001000;
    }
    if (car_config_36 > CC36_AdjustableSpeedLimiterType::Without_Adjustable_speed_limiter) {
        maxValue |= 0b00010000;
    }

    // Set configFlags
    vhal20::VehiclePropConfig config;
    config.prop = vhal20::toInt(prop);
    config.areaConfigs.resize(1);
    config.areaConfigs[0].areaId = 0;
    config.areaConfigs[0].minInt32Value = 0;
    config.areaConfigs[0].maxInt32Value = maxValue;

    return config;
}

DriverSupportFunctionModule::DriverSupportFunctionModule(gsl::not_null<VFContext*> ctx)
    : pa_driver_support_function_on(propconfig_driver_support_function_on(),
                                    vccvhal10::VehicleProperty::DRIVER_SUPPORT_FUNCTION_STATUS,
                                    ctx->dispatcher,
                                    &(ctx->vhal)),
      setting_(SettingId::DriverSuprtFct_On, DrvrSpprtFct::CC, ctx->settings),
      vehmod_flexray_receiver_{ctx->dataelements},
      crsCtrlrSts_flexray_receiver_{ctx->dataelements},
      accadpr_flexray_receiver_{ctx->dataelements},
      adj_spd_limn_sts_receiver_{ctx->dataelements},
      vhal_acc_enabled{false},
      vhal_cc_enabled{false},
      vhal_sl_enabled{false},
      acc_sts_off{false},
      cc_sts_off{false},
      sl_sts_off{false},
      is_active_{true},
      drvr_spprt_func_status{vccvhal10::PAStatus::Disabled} {
    // Check carconfig
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_36 = carconfig::getValue<CC36_AdjustableSpeedLimiterType>();

    // Check what modes are enabled
    if (car_config_23 > CC23_CruiseControlType::No_Cruise_control) {
        vhal_cc_enabled = true;
    }
    if (car_config_23 > CC23_CruiseControlType::Cruise_control) {
        vhal_acc_enabled = true;
    }
    if (car_config_36 > CC36_AdjustableSpeedLimiterType::Without_Adjustable_speed_limiter) {
        vhal_sl_enabled = true;
    }

    // Check if at least two of the settings are supported.
    const bool is_enabled_ = vhal_acc_enabled || (vhal_cc_enabled && vhal_sl_enabled);

    if (!is_enabled_) {
        ALOGD("Function disabled due to CarConfig.");
        drvr_spprt_func_on_ = vhal_sl_enabled ? DrvrSpprtFct::SL : DrvrSpprtFct::CC;
        SetFlexray(drvr_spprt_func_on_);
        return;
    }

    // Start listen to Settings
    setting_.setCallback([this](const auto& value) {
        auto callbackValue = value.value;
        ALOGD("Setting is changed: %d", callbackValue);

        if ((callbackValue == DrvrSpprtFct::ACC) && vhal_acc_enabled) {
            drvr_spprt_func_on_ = DrvrSpprtFct::ACC;
        } else if (callbackValue == DrvrSpprtFct::SL) {
            if (vhal_sl_enabled) {
                drvr_spprt_func_on_ = DrvrSpprtFct::SL;
            } else {
                drvr_spprt_func_on_ = DrvrSpprtFct::ACC;
            }
        } else {
            drvr_spprt_func_on_ = DrvrSpprtFct::CC;
        }

        Update();
    });

    // Register to VHAL
    pa_driver_support_function_on.registerToVehicleHal();
    pa_driver_support_function_on.subscribe_set_prop([&](int32_t value, int32_t zone) {
        (void)zone;
        ALOGV("VHAL is changed, (Update->Flexray+Setting): %d", value);
        if (value == DriverSupportFunc::ACC) {
            drvr_spprt_func_on_ = DrvrSpprtFct::ACC;
        } else if (value == DriverSupportFunc::CC) {
            drvr_spprt_func_on_ = DrvrSpprtFct::CC;
        } else {
            drvr_spprt_func_on_ = DrvrSpprtFct::SL;
        }
        Update();
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_active_ = (vehmod_value.UsgModSts == UsgModSts1::UsgModActv ||
                          vehmod_value.UsgModSts == UsgModSts1::UsgModCnvinc ||
                          vehmod_value.UsgModSts == UsgModSts1::UsgModDrvg);

            ALOGV("is_active_ is changed: %d", is_active_);
            Update();
        }
    });

    accadpr_flexray_receiver_.subscribe([&]() {
        const auto signal_accadpr = accadpr_flexray_receiver_.get();
        if (signal_accadpr.isOk()) {
            auto adaptive_cruise_ctrl_status_value = signal_accadpr.value();
            acc_sts_off = ((adaptive_cruise_ctrl_status_value == AccSts1::Actv) ||
                           (adaptive_cruise_ctrl_status_value == AccSts1::Stop) ||
                           (adaptive_cruise_ctrl_status_value == AccSts1::StopTmp));
            Update();
        }
    });

    crsCtrlrSts_flexray_receiver_.subscribe([&]() {
        const auto signal_crsCtrlr = crsCtrlrSts_flexray_receiver_.get();
        if (signal_crsCtrlr.isOk()) {
            auto cruise_ctrl_status_value = signal_crsCtrlr.value();
            cc_sts_off = (cruise_ctrl_status_value == CrsCtrlrSts1::Actv);
            Update();
        }
    });

    adj_spd_limn_sts_receiver_.subscribe([&]() {
        const auto signal_adjSpd = adj_spd_limn_sts_receiver_.get();
        if (signal_adjSpd.isOk()) {
            auto adj_speed_limiter_status_value = signal_adjSpd.value();
            sl_sts_off = ((adj_speed_limiter_status_value == AdjSpdLimnSts2::Actv) ||
                          (adj_speed_limiter_status_value == AdjSpdLimnSts2::Ovrd));
            Update();
        }
    });
}

void DriverSupportFunctionModule::Update() {
    if (!is_active_) {
        drvr_spprt_func_status = vccvhal10::PAStatus::Disabled;
        SetFlexray(drvr_spprt_func_on_);
    } else {
        if (!acc_sts_off && !cc_sts_off && !sl_sts_off) {
            drvr_spprt_func_status = vccvhal10::PAStatus::Active;
            SetFlexray(drvr_spprt_func_on_);
            SetVhal(drvr_spprt_func_on_, drvr_spprt_func_status);
        } else {
            ALOGV("Driver Support Function Flexray signal timed out");
            pa_driver_support_function_on.PushProp(false, vccvhal10::PAStatus::SystemError);
        }
    }
}

void DriverSupportFunctionModule::SetFlexray(DrvrSpprtFct value) {
    usrseld_drvr_SpprtFct_flexray_sender_.send(value);
    setting_.set(value);
}

// Set Vhal
void DriverSupportFunctionModule::SetVhal(DrvrSpprtFct value, vccvhal10::PAStatus status) {
    if (value == DrvrSpprtFct::CC) {
        pa_driver_support_function_on.PushProp(DriverSupportFunc::CC, status);
    } else if (value == DrvrSpprtFct::ACC) {
        pa_driver_support_function_on.PushProp(DriverSupportFunc::ACC, status);
    } else {
        pa_driver_support_function_on.PushProp(DriverSupportFunc::SL, status);
    }
}
