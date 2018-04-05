/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "e_lane_keeping_aid_module.h"

#undef LOG_TAG
#define LOG_TAG "ELaneKeepingAidModule"

vhal20::VehiclePropConfig propconfig_elane_keeping_aid_on() {
    auto prop = vccvhal10::VehicleProperty::EMERGENCY_LANE_KEEPING_AID_ON;
    return BoolConfig(prop);
}

ELaneKeepingAidModule::ELaneKeepingAidModule(vhal20::impl::IVehicleHalImpl* vehicleHal,
                                             std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                                             android::sp<SettingsFramework::SettingsManagerHidl> manager)
    : PA_prop_elane_keeping_aid(propconfig_elane_keeping_aid_on(),
                                vccvhal10::VehicleProperty::EMERGENCY_LANE_KEEPING_AID_STATUS,
                                dispatcher,
                                vehicleHal),
      setting_(SettingId::EmergencyLaneKeepingAid_On, true, manager) {
    // Check if enabled by carconfig
    auto car_config_150 = carconfig::getValue<CC150_LaneKeepingAidType>();
    auto car_config_316 = carconfig::getValue<CC316_EmergencyLaneKeepingAidType>();
    auto lcfg = vcc::LocalConfigDefault();

    const bool lcfg_RDP_OnOffStatus_Visible = lcfg->GetBool("vehiclefunctions.adas.RDP_OnOffStatus_Visible");

    const bool is_enabled_ =
            ((car_config_150 == CC150_LaneKeepingAidType::Lane_Keeping_Aid ||
              car_config_150 == CC150_LaneKeepingAidType::Lane_Departure_Warning) &&
             car_config_316 > CC316_EmergencyLaneKeepingAidType::Without_Emergency_Lane_Keeping_Aid_eLKA_ &&
             lcfg_RDP_OnOffStatus_Visible == true);

    if (!is_enabled_) {
        ALOGV("Function disabled due to CarConfig.");
        elane_keeping_aid_on_ = false;
        SetFlexray(elane_keeping_aid_on_, current_profile_);
        return;
    }

    // Start listen to Settings
    setting_.setCallback([this](const auto& value) {
        ALOGV("Setting is changed: %d", value.value);
        elane_keeping_aid_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to Vhal
    PA_prop_elane_keeping_aid.registerToVehicleHal();
    PA_prop_elane_keeping_aid.subscribe_set_prop([&](bool requestedELKA_On, int32_t zone) {
        (void)zone;
        ALOGV("VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", requestedELKA_On);
        setting_.set(requestedELKA_On);
    });

    // Check Alivetimeout for error
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

void ELaneKeepingAidModule::Update() {
    is_error_ = (is_alivetimeout_ || is_invalid_crvtspdadpv_signal_);

    if (is_error_) {
        SetFlexray(false, current_profile_);
        PA_prop_elane_keeping_aid.PushProp(false, vccvhal10::PAStatus::SystemError);  // Set Vhal
    } else {
        if (!is_active_) {
            SetFlexray(false, current_profile_);
            PA_prop_elane_keeping_aid.PushProp(false, vccvhal10::PAStatus::Disabled);  // Set Vhal
        } else {
            SetFlexray(elane_keeping_aid_on_, current_profile_);
            PA_prop_elane_keeping_aid.PushProp(elane_keeping_aid_on_, vccvhal10::PAStatus::Active);  // Set Vhal
        }
    }
}

void ELaneKeepingAidModule::SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);

    OnOffPen flexray_value;
    flexray_value.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value.Pen = flexray_profile;
    accadpr_turnspd_actv_flexray_sender_.send(flexray_value);
}
