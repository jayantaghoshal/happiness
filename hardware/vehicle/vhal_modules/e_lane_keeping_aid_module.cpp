/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "e_lane_keeping_aid_module.h"
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"
#include "modulepropertyhandler.h"
#include "vcc/localconfig.h"

#undef LOG_TAG
#define LOG_TAG "ELaneKeepingAidModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;
using namespace ApplicationDataElement;

vhal20::VehiclePropConfig propconfig_elane_keeping_aid_on() {
    auto prop = vccvhal10::VehicleProperty::EMERGENCY_LANE_KEEPING_AID_ON;
    return BoolConfig(prop);
}

ELaneKeepingAidModule::ELaneKeepingAidModule(vhal20::impl::IVehicleHalImpl* vehicleHal,
                                             std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                                             android::sp<SettingsFramework::SettingsManagerHidl> manager)
    : PA_prop_elane_keeping_aid_(propconfig_elane_keeping_aid_on(),
                                 vccvhal10::VehicleProperty::EMERGENCY_LANE_KEEPING_AID_STATUS,
                                 dispatcher,
                                 vehicleHal),
      setting_(SettingId::EmergencyLaneKeepingAid_On, true, manager),
      is_error_(false),
      is_active_(true),
      is_usgMod_Drvg_(false),
      current_profile_(SettingsFramework::ProfileIdentifier::None),
      is_eLKA_SrvRqrd_(false),
      is_alivetimeout_(false),
      is_invalid_signal_(false),
      is_set_to_Default_(false) {
    ALOGV("Starting Emergency Lane Keeping Aid ");

    auto car_config_150 = carconfig::getValue<CC150_LaneKeepingAidType>();
    auto car_config_220 = carconfig::getValue<CC220_LanguageRegionType>();
    auto car_config_316 = carconfig::getValue<CC316_EmergencyLaneKeepingAidType>();
    auto lcfg = vcc::LocalConfigDefault();

    const bool lcfg_RDP_OnOffStatus_Visible = lcfg->GetBool("vehiclefunctions.adas.RDP_OnOffStatus_Visible");
    const bool lcfg_RDP_OnOffDefaultSetting_ON = lcfg->GetBool("vehiclefunctions.adas.RDP_OnOffDefaultSetting_ON");

    const bool is_enabled_by_carconfig =
            ((car_config_150 == CC150_LaneKeepingAidType::Lane_Keeping_Aid ||
              car_config_150 == CC150_LaneKeepingAidType::Lane_Departure_Warning) &&
             car_config_316 > CC316_EmergencyLaneKeepingAidType::Without_Emergency_Lane_Keeping_Aid_eLKA_ &&
             lcfg_RDP_OnOffStatus_Visible == true);

    bool is_set_to_Default_Rqrd = (lcfg_RDP_OnOffDefaultSetting_ON == true ||
                                   (car_config_316 >= CC316_EmergencyLaneKeepingAidType::eLKA_level_3 &&
                                    car_config_220 == CC220_LanguageRegionType::Language_Region_for_Europe));

    is_set_to_Default_ = is_set_to_Default_Rqrd;

    // Check if enabled by carconfig
    if (!is_enabled_by_carconfig) {
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
    PA_prop_elane_keeping_aid_.registerToVehicleHal();
    PA_prop_elane_keeping_aid_.subscribe_set_prop([&](bool requestedELKA_On, int32_t zone) {
        (void)zone;
        ALOGV("VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", requestedELKA_On);
        if (is_set_to_Default_Rqrd && !is_set_to_Default_)
            setting_.set(requestedELKA_On);
        else {
            elane_keeping_aid_on_ = requestedELKA_On;
            Update();
        }
    });

    // Check for system errors
    lanekeepingaid_flexray_receiver_.subscribe([&]() {
        const auto signal = lanekeepingaid_flexray_receiver_.get();
        if (!signal.isOk()) {
            is_alivetimeout_ = true;

        } else {
            is_alivetimeout_ = false;
            auto lanekeepaidsts_value = signal.value();
            is_eLKA_SrvRqrd_ = (lanekeepaidsts_value == autosar::FctSts2::SrvRqrd);
            ALOGV("eLKA: is_invalid_signal is changed: %d", is_invalid_signal_);
        }
        Update();
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_usgMod_Drvg_ = vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg;

            is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModActv ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModCnvinc ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg);

            ALOGV("eLKA :: is_active_ : %d and is_set_to_Default_ : %d", is_active_, is_set_to_Default_);

            if (is_set_to_Default_Rqrd) {
                if (is_active_ && is_set_to_Default_) {
                    elane_keeping_aid_on_ = true;
                    setting_.set(true);
                    is_set_to_Default_ = false;
                } else if (!is_active_) {
                    is_set_to_Default_ = true;
                }
            }

            Update();
        }
    });
}

void ELaneKeepingAidModule::Update() {
    is_invalid_signal_ = is_eLKA_SrvRqrd_ && is_active_;
    is_error_ = ((is_usgMod_Drvg_ && is_alivetimeout_) || is_invalid_signal_);

    ALOGV("eLKA Update() :: is_active_----------- : %d ", is_active_);
    ALOGV("eLKA Update() :: is_eLKA_SrvRqrd_----- : %d ", is_eLKA_SrvRqrd_);
    ALOGV("eLKA Update() :: is_usgMod_Drvg_------ : %d ", is_usgMod_Drvg_);
    ALOGV("eLKA Update() :: is_alivetimeout_----- : %d ", is_alivetimeout_);
    ALOGV("eLKA Update() :: is_invalid_signal_--- : %d ", is_invalid_signal_);
    ALOGV("eLKA Update() :: elane_keeping_aid_on_ : %d ", elane_keeping_aid_on_);

    if (is_error_) {
        SetFlexray(false, current_profile_);
        PA_prop_elane_keeping_aid_.PushProp(false, vccvhal10::PAStatus::SystemError);  // Set Vhal
    } else {
        if (!is_active_) {
            SetFlexray(false, current_profile_);
            PA_prop_elane_keeping_aid_.PushProp(false, vccvhal10::PAStatus::Disabled);  // Set Vhal
        } else {
            SetFlexray(elane_keeping_aid_on_, current_profile_);
            PA_prop_elane_keeping_aid_.PushProp(elane_keeping_aid_on_, vccvhal10::PAStatus::Active);  // Set Vhal
        }
    }
}

void ELaneKeepingAidModule::SetFlexray(bool value, SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);

    OnOffPen flexray_value;
    flexray_value.Sts = value ? OnOff1::On : OnOff1::Off;
    flexray_value.Pen = flexray_profile;
    lanekeepingaid_flexray_sender_.send(flexray_value);
}
