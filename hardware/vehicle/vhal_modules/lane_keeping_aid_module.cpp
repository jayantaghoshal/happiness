/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "lane_keeping_aid_module.h"
#include <vhal_v2_0/VehicleUtils.h>
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

#undef LOG_TAG
#define LOG_TAG "LaneKeepingAid"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

vhal20::VehiclePropConfig propconfig_lane_keeping_aid_on() {
    auto prop = vccvhal10::VehicleProperty::LANE_KEEPING_AID_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

vhal20::VehiclePropConfig propconfig_lane_keeping_aid_mode() {
    auto prop = vccvhal10::VehicleProperty::LANE_KEEPING_AID_MODE;

    vhal20::VehiclePropConfig config;
    config.prop = vhal20::toInt(prop);
    config.access = vhal20::VehiclePropertyAccess::READ_WRITE;
    config.changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE;
    config.areaConfigs.resize(1);
    config.areaConfigs[0].areaId = 0;
    config.areaConfigs[0].minInt32Value = 0;
    config.areaConfigs[0].maxInt32Value = 2;
    return config;
}

LaneKeepingAidModule::LaneKeepingAidModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_lane_keeping_aid_on_(propconfig_lane_keeping_aid_on(),
                                   vccvhal10::VehicleProperty::LANE_KEEPING_AID_ON_STATUS,
                                   ctx->dispatcher,
                                   &(ctx->vhal)),
      PA_prop_lane_keeping_aid_mode_(propconfig_lane_keeping_aid_mode(),
                                     vccvhal10::VehicleProperty::LANE_KEEPING_AID_MODE_STATUS,
                                     ctx->dispatcher,
                                     &(ctx->vhal)),
      setting_lka_on_(SettingId::LaneKeepingAid_On, true, ctx->settings),
      setting_lka_mode_(SettingId::LaneKeepingAid_Mode, 0, ctx->settings),
      vehmod_flexray_receiver_{ctx->dataelements},
      lane_keep_aid_sts_receiver_{ctx->dataelements},
      is_error_(false),
      is_active_(true),
      lane_keeping_aid_on_(true),
      lane_keeping_aid_mode_(
              static_cast<std::underlying_type<autosar::WarnAndIntv1>::type>(autosar::WarnAndIntv1::WarnAndIntv)),
      is_invalid_lane_keep_aid_sts_signal_(false),
      is_alivetimeout_(false) {
    // Check if enabled.
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_150 = carconfig::getValue<CC150_LaneKeepingAidType>();
    const bool is_enabled_ =
            (car_config_23 > CC23_CruiseControlType::Cruise_control &&
             car_config_150 > CC150_LaneKeepingAidType::Without_Lane_Keeping_Aid_Lane_Departure_Warning);

    // Exit if feature is disabled in CarConfig.
    if (!is_enabled_) {
        ALOGD("LaneKeepingAid Function disabled due to CarConfig.");
        SetFlexray(false,
                   static_cast<std::underlying_type<WarnAndIntv1>::type>(WarnAndIntv1::WarnAndIntv),
                   current_profile_);
        return;
    }

    // Start listen to on/off setting.
    setting_lka_on_.setCallback([this](const auto& value) {
        ALOGD("On/Off setting is changed: %d", value.value);
        lane_keeping_aid_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to mode setting.
    setting_lka_mode_.setCallback([this](const auto& value) {
        ALOGD("Mode setting is changed!");
        lane_keeping_aid_mode_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to Vhal Lane Keeping Aid On/Off.
    PA_prop_lane_keeping_aid_on_.registerToVehicleHal();
    PA_prop_lane_keeping_aid_on_.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGD("VHAL is changed, (on/off), lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        lane_keeping_aid_on_ = value;
        setting_lka_on_.set(value);
    });

    // Start listen to Vhal Lane Keeping Aid mode.
    PA_prop_lane_keeping_aid_mode_.registerToVehicleHal();
    PA_prop_lane_keeping_aid_mode_.subscribe_set_prop([&](int mode, int32_t zone) {
        (void)zone;
        if (isModeInRange(mode)) {
            ALOGD("VHAL is changed, (mode), lets update (Setting->MainLoop->Vhal/Flexray): %d", mode);
            lane_keeping_aid_mode_ = mode;
            setting_lka_mode_.set(mode);
        } else {
            ALOGI("Lane Keeping Aid mode is out of range. Skipping.");
        }
    });

    // Check if function is active.
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModActv ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModCnvinc ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg);

            ALOGD("is_active_ is changed: %d", is_active_);
            Update();
        }
    });

    // Check system error.
    lane_keep_aid_sts_receiver_.subscribe([&]() {
        const auto signal = lane_keep_aid_sts_receiver_.get();
        if (!signal.isOk()) {
            ALOGD("Signal not Ok. Indicating alive timeout.");
            is_alivetimeout_ = true;

        } else {
            is_alivetimeout_ = false;
            ALOGD("Signal Ok. Alive timeout is now false");
            auto lanekeepaidsts_value = signal.value();
            is_invalid_lane_keep_aid_sts_signal_ = lanekeepaidsts_value == autosar::FctSts2::SrvRqrd;
            ALOGD("is_invalid_lane_keep_aid_sts_signal_ is changed: %d", is_invalid_lane_keep_aid_sts_signal_);
        }
        Update();
    });
}

void LaneKeepingAidModule::Update() {
    is_error_ = is_alivetimeout_ || is_invalid_lane_keep_aid_sts_signal_;

    int defaultWarnAndIntv = static_cast<std::underlying_type<WarnAndIntv1>::type>(WarnAndIntv1::WarnAndIntv);

    if (is_error_) {
        ALOGD("is_error_ is true. Indicating SystemError.");
        SetFlexray(false, defaultWarnAndIntv, current_profile_);
        PA_prop_lane_keeping_aid_on_.PushProp(false, vccvhal10::PAStatus::SystemError);
        PA_prop_lane_keeping_aid_mode_.PushProp(defaultWarnAndIntv, vccvhal10::PAStatus::SystemError);

    } else {
        if (!is_active_) {
            ALOGD("is_active_ is false. Setting props to default and sending default signals on Flexray.");
            SetFlexray(false, defaultWarnAndIntv, current_profile_);
            PA_prop_lane_keeping_aid_on_.PushProp(false, vccvhal10::PAStatus::Disabled);
            PA_prop_lane_keeping_aid_mode_.PushProp(defaultWarnAndIntv, vccvhal10::PAStatus::Disabled);

        } else {
            ALOGD("Updating LKA On/Off to %d", lane_keeping_aid_on_);

            SetFlexray(lane_keeping_aid_on_, lane_keeping_aid_mode_, current_profile_);
            PA_prop_lane_keeping_aid_on_.PushProp(lane_keeping_aid_on_, vccvhal10::PAStatus::Active);

            ALOGD("Updating LKA Mode to %d", lane_keeping_aid_mode_);
            PA_prop_lane_keeping_aid_mode_.PushProp(lane_keeping_aid_mode_, vccvhal10::PAStatus::Active);
        }
    }
}

void LaneKeepingAidModule::SetFlexray(bool value, int mode, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);
    // Not sure this is needed.
    activatevfc_.send({Vfc::UserInputSettings, 3});  // Make sure the CEM & FR is alive

    // Send On/Off.
    OnOffPen flexray_value_onoff;
    flexray_value_onoff.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value_onoff.Pen = flexray_profile;
    lanekeepaidactv_flexray_sender_.send(flexray_value_onoff);

    // Send mode.
    WarnAndIntvPen1 flexray_value_mode;
    flexray_value_mode.Sts = static_cast<WarnAndIntv1>(mode);
    flexray_value_mode.Pen = flexray_profile;
    intv_and_warnmod_for_lanekeepaid_flexray_sender_.send(flexray_value_mode);

    // Always send SoundHptc1::Hptc.
    WarnTypForLaneChgWarnPen1 flexray_warn;
    flexray_warn.Sts = SoundHptc1::Hptc;
    flexray_warn.Pen = flexray_profile;
    warntypforlanekeepaid_flexray_sender_.send(flexray_warn);
}

bool LaneKeepingAidModule::isModeInRange(int mode) {
    return (mode == static_cast<std::underlying_type<WarnAndIntv1>::type>(WarnAndIntv1::WarnAndIntv) ||
            mode == static_cast<std::underlying_type<WarnAndIntv1>::type>(WarnAndIntv1::Intv) ||
            mode == static_cast<std::underlying_type<WarnAndIntv1>::type>(WarnAndIntv1::Warn));
}
