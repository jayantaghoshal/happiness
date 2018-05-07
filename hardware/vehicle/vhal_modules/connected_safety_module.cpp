/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "connected_safety_module.h"
#include <vhal_v2_0/VehicleUtils.h>
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "i_vehicle_hal_impl.h"
#include "interface_mapping.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

#undef LOG_TAG
#define LOG_TAG "ConnectedSafety"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

vhal20::VehiclePropConfig propconfig_connected_safety_on() {
    auto prop = vccvhal10::VehicleProperty::CONNECTED_SAFETY_ON;
    return BoolConfig(prop);
}

ConnectedSafetyModule::ConnectedSafetyModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_connected_safety_on_(propconfig_connected_safety_on(),
                                   vccvhal10::VehicleProperty::CONNECTED_SAFETY_ON_STATUS,
                                   ctx->dispatcher,
                                   &(ctx->vhal)),
      setting_connected_safety_on_(SettingId::ConnectedSafety_On, true, ctx->settings),
      roadfricindcnsts_flexray_receiver_{ctx->dataelements},
      vehmod_flexray_receiver_{ctx->dataelements},
      is_error_(false),
      is_active_(true),
      connected_safety_on_(true),
      is_invalid_connected_safety_signal_(false),
      is_alivetimeout_(false) {
    // Check if enabled.
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_147 = carconfig::getValue<CC147_ConnectedSafetycsType>();

    const bool is_enabled_ = car_config_23 > CC23_CruiseControlType::Cruise_control &&
                             (car_config_147 == CC147_ConnectedSafetycsType::With_Connected_Safety_level_1 ||
                              car_config_147 == CC147_ConnectedSafetycsType::With_Connected_Safety_level_2);

    // Exit if feature is disabled in CarConfig.
    if (!is_enabled_) {
        ALOGD("Connected Safety Function disabled due to CarConfig.");
        SetFlexray(false, current_profile_);
        return;
    }

    // Start listen to on/off setting.
    setting_connected_safety_on_.setCallback([this](const auto& value) {
        ALOGD("On/Off setting is changed: %d", value.value);
        connected_safety_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to Vhal Connected Safety On/Off.
    PA_prop_connected_safety_on_.registerToVehicleHal();
    PA_prop_connected_safety_on_.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGD("VHAL is changed, (on/off), lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        connected_safety_on_ = value;
        setting_connected_safety_on_.set(value);
    });

    // Check if function is active.
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_active_ = (vehmod_value.UsgModSts == UsgModSts1::UsgModActv ||
                          vehmod_value.UsgModSts == UsgModSts1::UsgModCnvinc ||
                          vehmod_value.UsgModSts == UsgModSts1::UsgModDrvg);

            ALOGD("is_active_ is changed: %d", is_active_);
            Update();
        }
    });

    // Check RoadFricIndcnSts Flexray signal.
    roadfricindcnsts_flexray_receiver_.subscribe([&]() {
        const auto signal = roadfricindcnsts_flexray_receiver_.get();
        if (!signal.isOk()) {
            ALOGD("Signal not Ok. Indicating alive timeout.");
            is_alivetimeout_ = true;

        } else {
            is_alivetimeout_ = false;
            ALOGD("Signal Ok. Alive timeout is now false");
            auto roadfricindcnsts_value = signal.value();
            connected_safety_on_ = (roadfricindcnsts_value == autosar::FctSts2::On);

            is_invalid_connected_safety_signal_ = (roadfricindcnsts_value == autosar::FctSts2::SrvRqrd);
            ALOGD("is_invalid_connected_safety_signal_ is changed: %d", is_invalid_connected_safety_signal_);
        }
        Update();
    });
}

void ConnectedSafetyModule::Update() {
    is_error_ = is_alivetimeout_ || is_invalid_connected_safety_signal_;

    if (is_error_) {
        ALOGD("is_error_ is true. Indicating SystemError.");
        SetFlexray(false, current_profile_);
        PA_prop_connected_safety_on_.PushProp(false, vccvhal10::PAStatus::SystemError);

    } else {
        if (!is_active_) {
            ALOGD("is_active_ is false. Setting props to default and sending default signals on Flexray.");
            SetFlexray(false, current_profile_);
            PA_prop_connected_safety_on_.PushProp(false, vccvhal10::PAStatus::Disabled);

        } else {
            ALOGD("Updating Connected Safety On/Off to %d", connected_safety_on_);

            SetFlexray(connected_safety_on_, current_profile_);
            PA_prop_connected_safety_on_.PushProp(connected_safety_on_, vccvhal10::PAStatus::Active);
        }
    }
}

void ConnectedSafetyModule::SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);
    // Keep flexray active.
    activatevfc_.send({Vfc::UserInputSettings, 3});  // Make sure the CEM & FR is alive

    // Send On/Off.
    OnOffPen flexray_value_onoff;
    flexray_value_onoff.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value_onoff.Pen = flexray_profile;
    roadfricindcnactv_flexray_sender_.send(flexray_value_onoff);
}
