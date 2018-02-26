/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "setting_factory.h"

SettingFactory::SettingFactory(android::sp<SettingsFramework::SettingsManager> sm) 
    : sm_{sm}
    , AirConditioner_(
          SettingId::Climate_AirConditioner,  FirstRowGen::AirConditionerState::AUTO, sm)
    , AirConditionerDyno_(
          SettingId::Climate_AirConditionerDyno,  FirstRowGen::AirConditionerState::OFF, sm)
    , AirConditionerProxy_(AirConditioner_, AirConditionerDyno_)
    , AirDistribution_(SettingId::Climate_AirDistribution,                                             
                       FirstRowGen::AirDistributionAngle::AUTO, sm)
    , AirDistributionDyno_(SettingId::Climate_AirDistributionDyno,
                           FirstRowGen::AirDistributionAngle::AUTO, sm)
    , AirDistributionProxy_(AirDistribution_, AirDistributionDyno_)
    , ManualRecirc_(SettingId::Climate_ManualRecirculation,
                    FirstRowGen::ManualRecircState::OFF, sm)
    , ManualRecircDyno_(SettingId::Climate_ManualRecirculationDyno,
                        FirstRowGen::ManualRecircState::OFF, sm)
    , ManualRecircProxy_(ManualRecirc_, ManualRecircDyno_)
    , LastAutoFanLevel_(
          SettingId::Climate_AutoFanLevel, FirstRowGen::FanLevelFrontValue::LVL3, sm)
    , LastAutoFanLevelDyno_(
          SettingId::Climate_AutoFanLevelDyno, FirstRowGen::FanLevelFrontValue::LVL3, sm)          
    , lastAutoFanLevelFrontProxy_(LastAutoFanLevel_, LastAutoFanLevelDyno_)          
    , LastFanLevel_(
          SettingId::Climate_FanLevel,  FirstRowGen::FanLevelFrontValue::LVL3, sm)
    , LastFanLevelDyno_(
          SettingId::Climate_FanLevelDyno, FirstRowGen::FanLevelFrontValue::OFF, sm)
    , lastFanLevelFrontProxy_(LastFanLevel_, LastFanLevelDyno_)
    , TemperatureDriver_(SettingId::Climate_TemperatureDriver,  22.0, sm)
    , TemperatureDynoDriver_(SettingId::Climate_TemperatureDynoDriver,  22.0, sm)
    , TemperatureDriverProxy_(TemperatureDriver_, TemperatureDynoDriver_)
    , TemperatureDriverHiLoN_(SettingId::Climate_TemperatureDriverHiLoN,
                              autosar::HmiCmptmtTSpSpcl::Norm, sm)
    , TemperatureDynoDriverHiLoN_(SettingId::Climate_TemperatureDynoDriverHiLoN,
                                  autosar::HmiCmptmtTSpSpcl::Norm, sm)
    , TemperatureDriverHiLoNProxy_(TemperatureDriverHiLoN_, TemperatureDynoDriverHiLoN_)
    , TemperaturePassenger_(SettingId::Climate_TemperaturePassenger,  22.0, sm)
    , TemperatureDynoPassenger_(SettingId::Climate_TemperatureDynoPassenger,  22.0, sm)
    , TemperaturePassengerProxy_(TemperaturePassenger_, TemperatureDynoPassenger_)
    , TemperaturePassengerHiLoN_(
          SettingId::Climate_TemperaturePassengerHiLoN, autosar::HmiCmptmtTSpSpcl::Norm, sm)
    , TemperatureDynoPassengerHiLoN_(SettingId::Climate_TemperatureDynoPassengerHiLoN,
                                     autosar::HmiCmptmtTSpSpcl::Norm, sm)
    , TemperaturePassengerHiLoNProxy_(TemperaturePassengerHiLoN_, TemperatureDynoPassengerHiLoN_)
    , seatHeatLevelDriver_(
          SettingId::Climate_SeatHeatLevelDriver, FirstRowGen::HeatLevel::OFF, sm)
    , seatHeatLevelDriverDyno_(
          SettingId::Climate_SeatHeatLevelDriverDyno,  FirstRowGen::HeatLevel::OFF, sm)
    , seatHeatLevelDriverProxy_(seatHeatLevelDriver_, seatHeatLevelDriverDyno_)
    , seatHeatLevelPassenger_(
          SettingId::Climate_SeatHeatLevelPassenger, FirstRowGen::HeatLevel::OFF, sm)
    , seatHeatLevelPassengerDyno_(
          SettingId::Climate_SeatHeatLevelPassengerDyno, FirstRowGen::HeatLevel::OFF, sm)
    , seatHeatLevelPassengerProxy_(seatHeatLevelPassenger_, seatHeatLevelPassengerDyno_)
    , steeringWheelHeatLevel_(
          SettingId::Climate_SteeringWheelHeatLevel, FirstRowGen::HeatLevel::OFF, sm)
    , steeringWheelHeatLevelDyno_(
          SettingId::Climate_SteeringWheelHeatLevelDyno,  FirstRowGen::HeatLevel::OFF, sm)
    , steeringWheelHeatLevelProxy_(steeringWheelHeatLevel_, steeringWheelHeatLevelDyno_)
    , seatVentLevelDriver_(
          SettingId::Climate_SeatVentLevelDriver, FirstRowGen::VentLevel::OFF, sm)
    , seatVentLevelDriverDyno_(
          SettingId::Climate_SeatVentLevelDriverDyno, FirstRowGen::VentLevel::OFF, sm)
    , seatVentLevelDriverProxy_(seatVentLevelDriver_, seatVentLevelDriverDyno_)
    , seatVentLevelPassenger_(
          SettingId::Climate_SeatVentLevelPassenger,  FirstRowGen::VentLevel::OFF, sm)
    , seatVentLevelPassengerDyno_(
          SettingId::Climate_SeatVentLevelPassengerDyno,  FirstRowGen::VentLevel::OFF, sm)
    , seatVentLevelPassengerProxy_(seatVentLevelPassenger_, seatVentLevelPassengerDyno_)
    , ionizer_{ SettingId::Climate_Ionizer,  true , sm}
    , ionizerDyno_{ SettingId::Climate_IonizerDyno,  false , sm}
    , ionizerProxy_{ ionizer_, ionizerDyno_ }
{
}

std::unique_ptr<SettingsProxy<bool, UserScope::USER, UserScope::USER>> SettingFactory::createTemperatureSyncProxy()
{
    return std::make_unique<SettingsProxy<bool, UserScope::USER, UserScope::USER>>(
        SettingsFramework::Setting<bool, UserScope::USER>{
            SettingId::Climate_TemperatureSync, false , sm_},
        SettingsFramework::Setting<bool, UserScope::USER>{
            SettingId::Climate_TemperatureSyncDyno,  false, sm_});
}
