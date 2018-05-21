/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "traffic_sign_information_module.h"
#undef LOG_TAG
#define LOG_TAG "TrafficSignInformation"

using namespace CarConfigParams;
using namespace autosar;

namespace {
// Create propconfig function for TSI
vhal20::VehiclePropConfig propconfig_tsi_setting_on() {
    auto prop = vccvhal10::VehicleProperty::TSI_RSI_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

vhal20::VehiclePropConfig propconfig_tsi_speed_visual_warn_on() {
    auto prop = vccvhal10::VehicleProperty::TSI_SPEED_VISUAL_WARN_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

vhal20::VehiclePropConfig propconfig_tsi_speed_audio_warn_on() {
    auto prop = vccvhal10::VehicleProperty::TSI_SPEED_AUDIO_WARN_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

vhal20::VehiclePropConfig propconfig_tsi_speedcam_audio_warn_on() {
    auto prop = vccvhal10::VehicleProperty::TSI_SPEEDCAM_AUDIO_WARN_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

vhal20::VehiclePropConfig propconfig_tsi_speed_warn_offset() {
    auto prop = vccvhal10::VehicleProperty::TSI_SPEED_WARN_OFFSET;

    vhal20::VehiclePropConfig config;
    config.prop = vhal20::toInt(prop);
    config.access = vhal20::VehiclePropertyAccess::READ_WRITE;
    config.changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE;
    config.areaConfigs.resize(1);
    config.areaConfigs[0].areaId = 0;
    config.areaConfigs[0].minInt32Value = 0;
    config.areaConfigs[0].maxInt32Value = 20;
    return config;
}
// The factory reset value for certain markets are ON by default and for the rest, it is OFF.
// Below is the list of markets which has ON/True as factory reset values.
bool is_TSI_Default_On_Country(CC149_RoadSignInformationType car_config_149) {
    CC149_RoadSignInformationType TSI_Default_On_Countries[] = {
            CC149_RoadSignInformationType::SWEDEN_TSI_without_SLA,
            CC149_RoadSignInformationType::SWEDEN_TSI_with_SLA,
            CC149_RoadSignInformationType::IRELAND_TSI_without_SLA,
            CC149_RoadSignInformationType::IRELAND_TSI_with_SLA,
            CC149_RoadSignInformationType::ENGLAND_TSI_without_SLA,
            CC149_RoadSignInformationType::ENGLAND_TSI_with_SLA,
            CC149_RoadSignInformationType::MID_EU_TSI_without_SLA,
            CC149_RoadSignInformationType::MID_EU_TSI_with_SLA,
            CC149_RoadSignInformationType::EAST_EUROPE_TSI_without_SLA,
            CC149_RoadSignInformationType::EAST_EUROPE_TSI_with_SLA,
            CC149_RoadSignInformationType::SOUTH_EU_TSI_without_SLA,
            CC149_RoadSignInformationType::SOUTH_EU_TSI_with_SLA,
            CC149_RoadSignInformationType::USA_TSI_without_SLA,
            CC149_RoadSignInformationType::USA_TSI_with_SLA,
            CC149_RoadSignInformationType::CANADA_TSI_without_SLA,
            CC149_RoadSignInformationType::CANADA_TSI_with_SLA,
            CC149_RoadSignInformationType::AUSTRALIA_TSI_without_SLA,
            CC149_RoadSignInformationType::AUSTRALIA_TSI_with_SLA,
            CC149_RoadSignInformationType::JAPAN_TSI_without_SLA,
            CC149_RoadSignInformationType::JAPAN_TSI_with_SLA,
            CC149_RoadSignInformationType::CHINA_TSI_without_SLA,
            CC149_RoadSignInformationType::CHINA_TSI_with_SLA};

    return (std::find(std::begin(TSI_Default_On_Countries), std::end(TSI_Default_On_Countries), car_config_149) !=
            std::end(TSI_Default_On_Countries));
}

// For certain markets, the NVM stored value has to be ON/True everytime the value is retreived.
// Below is the list of markets where TSI is ON by default regardless of what is stored when vehicletype is v316
// or more.
// This also means that the setting need not necesarily be stored to NVM, but its initial value has to be ON all
// the time.
bool is_init_NoNVM_based(CC1_VehicleTypeType car_config_1, CC149_RoadSignInformationType car_config_149) {
    CC149_RoadSignInformationType TSI_NoNVM_based_Countries[] = {
            CC149_RoadSignInformationType::SWEDEN_TSI_without_SLA,
            CC149_RoadSignInformationType::SWEDEN_TSI_with_SLA,
            CC149_RoadSignInformationType::IRELAND_TSI_without_SLA,
            CC149_RoadSignInformationType::IRELAND_TSI_with_SLA,
            CC149_RoadSignInformationType::ENGLAND_TSI_without_SLA,
            CC149_RoadSignInformationType::ENGLAND_TSI_with_SLA,
            CC149_RoadSignInformationType::MID_EU_TSI_without_SLA,
            CC149_RoadSignInformationType::MID_EU_TSI_with_SLA,
            CC149_RoadSignInformationType::EAST_EUROPE_TSI_without_SLA,
            CC149_RoadSignInformationType::EAST_EUROPE_TSI_with_SLA,
            CC149_RoadSignInformationType::SOUTH_EU_TSI_without_SLA,
            CC149_RoadSignInformationType::SOUTH_EU_TSI_with_SLA,
            CC149_RoadSignInformationType::AUSTRALIA_TSI_without_SLA,
            CC149_RoadSignInformationType::AUSTRALIA_TSI_with_SLA};

    bool is_NoNVM_based_region =
            (std::find(std::begin(TSI_NoNVM_based_Countries), std::end(TSI_NoNVM_based_Countries), car_config_149) !=
             std::end(TSI_NoNVM_based_Countries));
    bool is_V316_or_above = (car_config_1 >= CC1_VehicleTypeType::V316);
    return (is_V316_or_above && is_NoNVM_based_region);
}
}  // namespace

// Initialize the constructors
TrafficSignInformationModule::TrafficSignInformationModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_tsi_rsi_(propconfig_tsi_setting_on(),
                       vccvhal10::VehicleProperty::TSI_RSI_STATUS,
                       ctx->dispatcher,
                       &(ctx->vhal)),
      PA_prop_tsi_speed_visual_warn_(propconfig_tsi_speed_visual_warn_on(),
                                     vccvhal10::VehicleProperty::TSI_SPEED_VISUAL_WARN_STATUS,
                                     ctx->dispatcher,
                                     &(ctx->vhal)),
      PA_prop_tsi_speed_audio_warn_(propconfig_tsi_speed_audio_warn_on(),
                                    vccvhal10::VehicleProperty::TSI_SPEED_AUDIO_WARN_STATUS,
                                    ctx->dispatcher,
                                    &(ctx->vhal)),
      PA_prop_tsi_speedcam_audio_warn_(propconfig_tsi_speedcam_audio_warn_on(),
                                       vccvhal10::VehicleProperty::TSI_SPEEDCAM_AUDIO_WARN_STATUS,
                                       ctx->dispatcher,
                                       &(ctx->vhal)),
      PA_prop_tsi_speed_warn_offset_(propconfig_tsi_speed_warn_offset(),
                                     vccvhal10::VehicleProperty::TSI_SPEED_WARN_OFFSET_STATUS,
                                     ctx->dispatcher,
                                     &(ctx->vhal)),
      vehmod_flexray_receiver_{ctx->dataelements},
      road_sgn_info_sts_flexray_receiver_{ctx->dataelements},
      car_config_1(carconfig::getValue<CC1_VehicleTypeType>()),
      car_config_149(carconfig::getValue<CC149_RoadSignInformationType>()),
      setting_tsi_rsi_(SettingId::TrafficSignInfo_RsiOn, is_TSI_Default_On_Country(car_config_149), ctx->settings),
      setting_tsi_speedcamaudiowarn_(SettingId::TrafficSignInfo_SpeedCamAudioWarnOn, false, ctx->settings),
      setting_tsi_speedvisualwarn_(SettingId::TrafficSignInfo_SpeedVisualWarnOn, true, ctx->settings),
      setting_tsi_speedaudiowarn_(SettingId::TrafficSignInfo_SpeedAudioWarnOn, false, ctx->settings),
      setting_tsi_speedwarnoffset_(SettingId::TrafficSignInfo_SpeedWarnOffset, 0, ctx->settings),
      is_alivetimeout_(false),
      is_invalid_road_sgn_info_sts_signal_(false),
      is_error_(false),
      is_active_(false),
      isFirstRetreive(true) {
    // Check if enabled
    const bool is_enabled_ =
            (car_config_149 !=
                     CC149_RoadSignInformationType::Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_ &&
             car_config_149 != CC149_RoadSignInformationType::Not_allowed_varaint);

    if (!is_enabled_) {
        ALOGV("Function disabled due to CarConfig.");
        SetFlexray(false, false, false, 0, current_profile_);
        return;
    }

    // Start listen to Settings
    setting_tsi_rsi_.setCallback([this](const auto& value) {
        ALOGV("Main setting is changed: %d", value.value);
        if (is_init_NoNVM_based(car_config_1, car_config_149) && isFirstRetreive) {
            tsi_rsi_on_ = true;
        } else {
            tsi_rsi_on_ = value.value;
        }
        current_profile_ = value.profileId;
        Update();
    });

    setting_tsi_speedcamaudiowarn_.setCallback([this](const auto& value) {
        ALOGV("SpeedCam audio warn is changed: %d", value.value);
        tsi_speedcam_audio_warn_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    setting_tsi_speedvisualwarn_.setCallback([this](const auto& value) {
        ALOGV("SpeedLim visual warn is changed: %d", value.value);
        tsi_speed_visual_warn_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    setting_tsi_speedaudiowarn_.setCallback([this](const auto& value) {
        ALOGV("SpeedLim audio warn is changed: %d", value.value);
        tsi_speed_audio_warn_on_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    setting_tsi_speedwarnoffset_.setCallback([this](const auto& value) {
        ALOGV("SpeedLim offset is changed: %d", value.value);
        tsi_speed_warn_offset_ = value.value;
        current_profile_ = value.profileId;
        Update();
    });

    // Start listen to Vhal
    PA_prop_tsi_rsi_.registerToVehicleHal();
    PA_prop_tsi_rsi_.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGV("Main Setting VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        setting_tsi_rsi_.set(value);
    });

    PA_prop_tsi_speed_visual_warn_.registerToVehicleHal();
    PA_prop_tsi_speed_visual_warn_.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGV("SpeedLim Visual warn VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        setting_tsi_speedvisualwarn_.set(value);
    });

    PA_prop_tsi_speed_audio_warn_.registerToVehicleHal();
    PA_prop_tsi_speed_audio_warn_.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGV("SpeedLim audio warn VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        setting_tsi_speedaudiowarn_.set(value);
    });

    PA_prop_tsi_speedcam_audio_warn_.registerToVehicleHal();
    PA_prop_tsi_speedcam_audio_warn_.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGV("SpeedCam audio warn VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        setting_tsi_speedcamaudiowarn_.set(value);
    });

    PA_prop_tsi_speed_warn_offset_.registerToVehicleHal();
    PA_prop_tsi_speed_warn_offset_.subscribe_set_prop([&](int value, int32_t zone) {
        (void)zone;
        ALOGV("SpeedLim offset VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", value);
        setting_tsi_speedwarnoffset_.set(value);
    });

    // Check Alivetimout for error and road_sgn_info_sts for error
    road_sgn_info_sts_flexray_receiver_.subscribe([&]() {
        const auto signal = road_sgn_info_sts_flexray_receiver_.get();
        is_alivetimeout_ = !signal.isOk();
        ALOGV("is_alivetimeout_ is changed: %d", is_alivetimeout_);
        if (signal.isOk()) {
            auto road_sgn_info_sts = signal.value();
            is_invalid_road_sgn_info_sts_signal_ = (road_sgn_info_sts == autosar::FctSts2::SrvRqrd);
            ALOGV("is_invalid_road_sgn_info_sts_signal_ is changed: %d", is_invalid_road_sgn_info_sts_signal_);
        }
        Update();
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();
            is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModActv ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModCnvinc ||
                          vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg);
            ALOGV("is_active_ is changed: %d", is_active_);
            if (is_active_ && is_init_NoNVM_based(car_config_1, car_config_149) && isFirstRetreive) {
                tsi_rsi_on_ = true;
            }
            Update();
        }
    });
}

// Update functionality here
void TrafficSignInformationModule::Update() {
    bool roadSgnInfo, spdLimVisWarning, spdLimAudWarning, spdCamAudWarning;
    roadSgnInfo = spdLimVisWarning = spdLimAudWarning = spdCamAudWarning = false;
    int spdLimWarnOffst = 0;
    vccvhal10::PAStatus PA_Status;

    is_error_ = (is_alivetimeout_ || is_invalid_road_sgn_info_sts_signal_);

    if (is_error_) {
        PA_Status = vccvhal10::PAStatus::SystemError;
    } else {
        if (!is_active_) {
            PA_Status = vccvhal10::PAStatus::Disabled;
            isFirstRetreive = true;
        } else {
            PA_Status = vccvhal10::PAStatus::Active;
            roadSgnInfo = tsi_rsi_on_;
            if (roadSgnInfo == true) {
                spdLimVisWarning = tsi_speed_visual_warn_on_;
                spdLimAudWarning = spdLimVisWarning ? tsi_speed_audio_warn_on_ : false;
                spdCamAudWarning = tsi_speedcam_audio_warn_on_;
                spdLimWarnOffst = tsi_speed_warn_offset_;
            }
            isFirstRetreive = false;
        }
    }
    PA_prop_tsi_rsi_.PushProp(roadSgnInfo, PA_Status);
    PA_prop_tsi_speed_visual_warn_.PushProp(spdLimVisWarning, PA_Status);
    PA_prop_tsi_speed_audio_warn_.PushProp(spdLimAudWarning, PA_Status);
    PA_prop_tsi_speedcam_audio_warn_.PushProp(spdCamAudWarning, PA_Status);
    PA_prop_tsi_speed_warn_offset_.PushProp(spdLimWarnOffst, PA_Status);
    SetFlexray(roadSgnInfo, spdLimVisWarning, spdCamAudWarning, spdLimWarnOffst, current_profile_);
}

// Define set flexray function
void TrafficSignInformationModule::SetFlexray(bool roadSgnInfo,
                                              bool spdLimVisWarning,
                                              bool spdCamAudWarning,
                                              int spdLimWarnOffst,
                                              SettingsFramework::ProfileIdentifier current_profile) {
    auto flexray_profile = InterfaceMapping::ProfileIdentifier::SettingsFramework_To_Flexray(current_profile);

    OnOffPen flexray_stsValue;
    flexray_stsValue.Pen = flexray_profile;

    flexray_stsValue.Sts = roadSgnInfo ? OnOff1::On : OnOff1::Off;
    road_sgn_info_actv_flexray_sender_.send(flexray_stsValue);

    flexray_stsValue.Sts = spdLimVisWarning ? OnOff1::On : OnOff1::Off;
    spd_alrm_actv_road_sgn_flexray_sender_.send(flexray_stsValue);

    flexray_stsValue.Sts = spdCamAudWarning ? OnOff1::On : OnOff1::Off;
    spdcam_info_seld_roadsgn_flexray_sender_.send(flexray_stsValue);

    OffsForSpdWarnSetgPen flexray_offstValue;
    flexray_offstValue.Pen = flexray_profile;

    flexray_offstValue.Sts = spdLimWarnOffst;
    offs_spd_warn_flexray_sender_.send(flexray_offstValue);
}
