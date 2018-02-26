/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <gen_datatypes.h>
#include <libsettings/setting.h>
#include "settings_proxy.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

using namespace SettingsFramework;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;




class SettingFactory
{

public:
    SettingFactory(android::sp<SettingsFramework::SettingsManager> sm);

private:
    android::sp<SettingsFramework::SettingsManager> sm_;

public:
    std::unique_ptr<SettingsProxy<bool, UserScope::USER, UserScope::USER>> createTemperatureSyncProxy();

    SettingsFramework::Setting<FirstRowGen::AirConditionerState::Literal, UserScope::USER> AirConditioner_;
    SettingsFramework::Setting<FirstRowGen::AirConditionerState::Literal, UserScope::NOT_USER_RELATED> AirConditionerDyno_;
    SettingsProxy<FirstRowGen::AirConditionerState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              AirConditionerProxy_;

    SettingsFramework::Setting<FirstRowGen::AirDistributionAngle::Literal, UserScope::USER> AirDistribution_;
    SettingsFramework::Setting<FirstRowGen::AirDistributionAngle::Literal, UserScope::NOT_USER_RELATED> AirDistributionDyno_;
    SettingsProxy<FirstRowGen::AirDistributionAngle::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              AirDistributionProxy_;

    SettingsFramework::Setting<FirstRowGen::ManualRecircState::Literal, UserScope::USER> ManualRecirc_;
    SettingsFramework::Setting<FirstRowGen::ManualRecircState::Literal, UserScope::NOT_USER_RELATED> ManualRecircDyno_;
    SettingsProxy<FirstRowGen::ManualRecircState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              ManualRecircProxy_;

    SettingsFramework::Setting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER> LastAutoFanLevel_;
    SettingsFramework::Setting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::NOT_USER_RELATED> LastAutoFanLevelDyno_;
    SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              lastAutoFanLevelFrontProxy_;

    SettingsFramework::Setting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER> LastFanLevel_;
    SettingsFramework::Setting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::NOT_USER_RELATED> LastFanLevelDyno_;
    SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              lastFanLevelFrontProxy_;

    SettingsFramework::Setting<double, UserScope::USER> TemperatureDriver_;
    SettingsFramework::Setting<double, UserScope::NOT_USER_RELATED> TemperatureDynoDriver_;
    SettingsProxy<double, UserScope::USER, UserScope::NOT_USER_RELATED>              TemperatureDriverProxy_;

    SettingsFramework::Setting<autosar::HmiCmptmtTSpSpcl, UserScope::USER> TemperatureDriverHiLoN_;
    SettingsFramework::Setting<autosar::HmiCmptmtTSpSpcl, UserScope::NOT_USER_RELATED> TemperatureDynoDriverHiLoN_;
    SettingsProxy<autosar::HmiCmptmtTSpSpcl, UserScope::USER, UserScope::NOT_USER_RELATED>              TemperatureDriverHiLoNProxy_;

    SettingsFramework::Setting<double, UserScope::NOT_USER_RELATED> TemperaturePassenger_;
    SettingsFramework::Setting<double, UserScope::NOT_USER_RELATED> TemperatureDynoPassenger_;
    SettingsProxy<double,UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>              TemperaturePassengerProxy_;

    SettingsFramework::Setting<autosar::HmiCmptmtTSpSpcl, UserScope::NOT_USER_RELATED> TemperaturePassengerHiLoN_;
    SettingsFramework::Setting<autosar::HmiCmptmtTSpSpcl, UserScope::NOT_USER_RELATED> TemperatureDynoPassengerHiLoN_;
    SettingsProxy<autosar::HmiCmptmtTSpSpcl,UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>              TemperaturePassengerHiLoNProxy_;

    SettingsFramework::Setting<FirstRowGen::HeatLevel::Literal, UserScope::USER> seatHeatLevelDriver_;
    SettingsFramework::Setting<FirstRowGen::HeatLevel::Literal, UserScope::NOT_USER_RELATED> seatHeatLevelDriverDyno_;
    SettingsProxy<FirstRowGen::HeatLevel::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              seatHeatLevelDriverProxy_;

    SettingsFramework::Setting<FirstRowGen::HeatLevel::Literal, UserScope::NOT_USER_RELATED> seatHeatLevelPassenger_;
    SettingsFramework::Setting<FirstRowGen::HeatLevel::Literal, UserScope::NOT_USER_RELATED> seatHeatLevelPassengerDyno_;
    SettingsProxy<FirstRowGen::HeatLevel::Literal,UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>              seatHeatLevelPassengerProxy_;

    SettingsFramework::Setting<FirstRowGen::HeatLevel::Literal, UserScope::USER> steeringWheelHeatLevel_;
    SettingsFramework::Setting<FirstRowGen::HeatLevel::Literal, UserScope::NOT_USER_RELATED> steeringWheelHeatLevelDyno_;
    SettingsProxy<FirstRowGen::HeatLevel::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              steeringWheelHeatLevelProxy_;

    SettingsFramework::Setting<FirstRowGen::VentLevel::Literal, UserScope::USER> seatVentLevelDriver_;
    SettingsFramework::Setting<FirstRowGen::VentLevel::Literal, UserScope::NOT_USER_RELATED> seatVentLevelDriverDyno_;
    SettingsProxy<FirstRowGen::VentLevel::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>              seatVentLevelDriverProxy_;

    SettingsFramework::Setting<FirstRowGen::VentLevel::Literal, UserScope::NOT_USER_RELATED> seatVentLevelPassenger_;
    SettingsFramework::Setting<FirstRowGen::VentLevel::Literal, UserScope::NOT_USER_RELATED> seatVentLevelPassengerDyno_;
    SettingsProxy<FirstRowGen::VentLevel::Literal, UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>              seatVentLevelPassengerProxy_;

    SettingsFramework::Setting<bool, UserScope::NOT_USER_RELATED> ionizer_;
    SettingsFramework::Setting<bool, UserScope::NOT_USER_RELATED> ionizerDyno_;
    SettingsProxy<bool,UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>              ionizerProxy_;
};
