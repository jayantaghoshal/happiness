/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"
#include "papropertyhandler.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class TrafficSignInformationModule {
  public:
    TrafficSignInformationModule(vhal20::impl::IVehicleHalImpl*,
                                 std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                                 android::sp<SettingsFramework::SettingsManagerHidl> manager);

  private:
    // Functions
    void Update();
    void SetPA_Props(bool roadSgnInfo,
                     bool spdLimVisWarning,
                     bool spdLimAudWarning,
                     bool spdCamAudWarning,
                     int spdLimWarnOffst,
                     vccvhal10::PAStatus PA_Status);
    void SetFlexray(bool roadSgnInfo,
                    bool spdLimVisWarning,
                    bool spdCamAudWarning,
                    int spdLimWarnOffst,
                    SettingsFramework::ProfileIdentifier current_profile);

    // Public attributes properties
    PAPropHandler<bool> PA_prop_tsi_rsi_;
    PAPropHandler<bool> PA_prop_tsi_speed_visual_warn_;
    PAPropHandler<bool> PA_prop_tsi_speed_audio_warn_;
    PAPropHandler<bool> PA_prop_tsi_speedcam_audio_warn_;
    PAPropHandler<int> PA_prop_tsi_speed_warn_offset_;

    // System signals
    ApplicationDataElement::DESender<autosar::RoadSgnInfoActv_info> road_sgn_info_actv_flexray_sender_;
    ApplicationDataElement::DESender<autosar::SpdCameraInfoSeldForRoadSgnInfo_info>
            spdcam_info_seld_roadsgn_flexray_sender_;
    ApplicationDataElement::DESender<autosar::SpdAlrmActvForRoadSgnInfo_info> spd_alrm_actv_road_sgn_flexray_sender_;
    ApplicationDataElement::DESender<autosar::OffsForSpdWarnSetg_info> offs_spd_warn_flexray_sender_;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    ApplicationDataElement::DEReceiver<autosar::RoadSgnInfoSts_info> road_sgn_info_sts_flexray_receiver_;

    // Carconfigs
    CarConfigParams::CC1_VehicleTypeType car_config_1;
    CarConfigParams::CC149_RoadSignInformationType car_config_149;

    // Setting storage
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_tsi_rsi_;
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_tsi_speedcamaudiowarn_;
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_tsi_speedvisualwarn_;
    SettingsFramework::Setting<bool, SettingsFramework::UserScope::USER> setting_tsi_speedaudiowarn_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> setting_tsi_speedwarnoffset_;
    SettingsFramework::ProfileIdentifier current_profile_ = SettingsFramework::ProfileIdentifier::None;

    // States
    bool tsi_rsi_on_;
    bool tsi_speed_visual_warn_on_;
    bool tsi_speed_audio_warn_on_;
    bool tsi_speedcam_audio_warn_on_;
    int tsi_speed_warn_offset_;

    // Sub States
    bool is_alivetimeout_;
    bool is_invalid_road_sgn_info_sts_signal_;

    // Members
    bool is_error_;
    bool is_active_;
    bool isFirstRetreive;
};
