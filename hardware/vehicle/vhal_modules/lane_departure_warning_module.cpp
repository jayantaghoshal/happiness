/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "lane_departure_warning_module.h"

#undef LOG_TAG
#define LOG_TAG "ELaneKeepingAidModule"

using namespace CarConfigParams;
using namespace autosar;

vhal20::VehiclePropConfig propconfig_lane_departure_warning_on() {
    auto prop = vccvhal10::VehicleProperty::LANE_DEPARTURE_WARNING_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

LaneDepartureWarningModule::LaneDepartureWarningModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_lane_departure_warning(propconfig_lane_departure_warning_on(),
                                     vccvhal10::VehicleProperty::LANE_DEPARTURE_WARNING_STATUS,
                                     ctx->dispatcher,
                                     &(ctx->vhal)),
      setting_(SettingId::LaneDepartureWarning_On, true, ctx->settings),
      vehmod_flexray_receiver_{ctx->dataelements},
      lanechngwarnsts_flexray_receiver_{ctx->dataelements},
      is_driving(false),
      is_alivetimeout_(false),
      ldw_service_req(false),
      is_active_(false) {
    // Check if enabled by carconfig
    auto car_config_150 = carconfig::getValue<CC150_LaneKeepingAidType>();

    const bool is_enabled_ = (car_config_150 == CC150_LaneKeepingAidType::Lane_Departure_Warning);

    if (!is_enabled_) {
        ALOGV("Function disabled due to CarConfig.");
        lane_departure_warning_on_ = false;
        SetFlexray(lane_departure_warning_on_, current_profile_);
        return;
    }

    // Start listen to Settings
    setting_.setCallback([this](const auto& value) {
        ALOGV("Setting is changed: %d", value.value);
        lane_departure_warning_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to Vhal
    PA_prop_lane_departure_warning.registerToVehicleHal();
    PA_prop_lane_departure_warning.subscribe_set_prop([&](bool requestedLDW_On, int32_t zone) {
        (void)zone;
        ALOGV("VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", requestedLDW_On);
        setting_.set(requestedLDW_On);
    });

    // Check Alivetimeout for error
    lanechngwarnsts_flexray_receiver_.subscribe([&]() {
        const auto signal = lanechngwarnsts_flexray_receiver_.get();
        is_alivetimeout_ = !signal.isOk();
        if (!is_alivetimeout_) {
            auto signal_value = signal.value();
            ldw_service_req = (signal_value == autosar::FctSts2::SrvRqrd);
        }
        ALOGV("is_alivetimeout_ is changed: %d", is_alivetimeout_);
        Update();
    });

    // Check if function is active State
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModActv ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModCnvinc);

            is_driving = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg);
            ALOGV("is_active_ is changed: %d", is_active_);
            Update();
        }
    });
}

void LaneDepartureWarningModule::Update() {
    // TimeOut monitoring only in usuage Mode Driving
    bool is_error_ = (is_driving && is_alivetimeout_);
    // Service Req Monitoring in Active State
    bool is_service_req = (is_active_ && ldw_service_req);
    if (is_error_ || is_service_req) {
        SetFlexray(false, current_profile_);
        PA_prop_lane_departure_warning.PushProp(false, vccvhal10::PAStatus::SystemError);  // Set Vhal
    } else {
        if (!is_active_) {
            SetFlexray(false, current_profile_);
            PA_prop_lane_departure_warning.PushProp(false, vccvhal10::PAStatus::Disabled);  // Set Vhal
        } else {
            SetFlexray(lane_departure_warning_on_, current_profile_);
            PA_prop_lane_departure_warning.PushProp(lane_departure_warning_on_,
                                                    vccvhal10::PAStatus::Active);  // Set Vhal
        }
    }
}

void LaneDepartureWarningModule::SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);

    OnOffPen flexray_value;
    WarnTypForLaneChgWarnPen1 flexray_warn;

    // Send ON OFF
    flexray_value.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value.Pen = flexray_profile;
    ldw_actv_flexray_sender_.send(flexray_value);

    // Always send SoundHptc1::Hptc.
    flexray_warn.Sts = SoundHptc1::Hptc;
    flexray_warn.Pen = flexray_profile;
    warntyp_for_ldw_flexray_sender_.send(flexray_warn);
}
