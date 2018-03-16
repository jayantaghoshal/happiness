/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "setting_factory.h"
#include <libsettings/setting.h>
#include "v0/org/volvocars/climate/UserSelection.hpp"

namespace user_selection {
namespace setting_factory {
using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using namespace SettingsFramework;

UserSelectionSettingFactory::UserSelectionSettingFactory(const android::sp<SettingsFramework::SettingsManager>& sm)
    : autoDriverSeatHeat_{SettingId::Climate_AutoDriverSeatHeat, UserSelectionGen::OffOnType::OFF, sm},
      autoDriverSeatHeatDyno_{SettingId::Climate_AutoDriverSeatHeatDyno, UserSelectionGen::OffOnType::OFF, sm},
      autoDriverSeatHeatLevel_{SettingId::Climate_AutoDriverSeatHeatLevel, UserSelectionGen::LevelType::LO, sm},
      autoDriverSeatHeatLevelDyno_{SettingId::Climate_AutoDriverSeatHeatLevelDyno, UserSelectionGen::LevelType::LO, sm},
      passengerSeatHeat_{SettingId::Climate_AutoPassengerSeatHeat, UserSelectionGen::OffOnType::OFF, sm},
      passengerSeatHeatDyno_{SettingId::Climate_AutoPassengerSeatHeatDyno, UserSelectionGen::OffOnType::OFF, sm},
      passengerSeatHeatLevel_{SettingId::Climate_AutoPassengerSeatHeatLevel, UserSelectionGen::LevelType::LO, sm},
      passengerSeatHeatLevelDyno_{SettingId::Climate_AutoPassengerSeatHeatLevelDyno,
                                  UserSelectionGen::LevelType::LO,
                                  sm},
      autoSteeringWheelHeat_{SettingId::Climate_AutoSteeringWheelHeat, UserSelectionGen::OffOnType::OFF, sm},
      autoSteeringWheelHeatDyno_{SettingId::Climate_AutoSteeringWheelHeatDyno, UserSelectionGen::OffOnType::OFF, sm},
      autoSteeringWheelHeatLevel_{SettingId::Climate_AutoSteeringWheelHeatLevel, UserSelectionGen::LevelType::LO, sm},
      autoSteeringWheelHeatLevelDyno_{SettingId::Climate_AutoSteeringWheelHeatLevelDyno,
                                      UserSelectionGen::LevelType::LO,
                                      sm},
      airQualitySensor_{SettingId::Climate_AirQualitySensor, static_cast<int>(autosar::OnOff1::On), sm},
      airQualitySensorDyno_{SettingId::Climate_AirQualitySensorDyno, static_cast<int>(autosar::OnOff1::On), sm},
      manualRecircTimer_{SettingId::Climate_ManualRecircTimer, static_cast<int>(autosar::OnOff1::Off), sm},
      manualRecircTimerDyno_{SettingId::Climate_ManualRecircTimerDyno, static_cast<int>(autosar::OnOff1::Off), sm},
      preconditioningHeatSource_{SettingId::Climate_PreconditioningHeatSource,
                                 UserSelectionGen::PreconditioningHeatSourceEnum::AUTO,
                                 sm},
      preconditioningHeatSourceDyno_{SettingId::Climate_PreconditioningHeatSourceDyno,
                                     UserSelectionGen::PreconditioningHeatSourceEnum::AUTO,
                                     sm},
      autoFrontDefroster_{SettingId::Climate_AutoFrontDefroster, UserSelectionGen::OffOnType::OFF, sm},
      autoFrontDefrosterDyno_{SettingId::Climate_AutoFrontDefrosterDyno, UserSelectionGen::OffOnType::OFF, sm},
      autoRearDefroster_{SettingId::Climate_AutoRearDefroster, UserSelectionGen::OffOnType::OFF, sm},
      autoRearDefrosterDyno_{SettingId::Climate_AutoRearDefrosterDyno, UserSelectionGen::OffOnType::OFF, sm},
      additionalHeater_{SettingId::Climate_AdditionalHeater, UserSelectionGen::OffOnType::ON, sm},
      additionalHeaterDyno_{SettingId::Climate_AdditionalHeaterDyno, UserSelectionGen::OffOnType::ON, sm} {}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoDriverSeatHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;
    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(autoDriverSeatHeat_,
                                                                                              autoDriverSeatHeatDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoDriverSeatHeatLevelProxy(
        const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(
            autoDriverSeatHeatLevel_, autoDriverSeatHeatLevelDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoPassengerSeatHeatProxy(
        const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(passengerSeatHeat_,
                                                                                              passengerSeatHeatDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoPassengerSeatHeatLevelProxy(
        const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(
            passengerSeatHeatLevel_, passengerSeatHeatLevelDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoSteeringWheelHeatProxy(
        const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(
            autoSteeringWheelHeat_, autoSteeringWheelHeatDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoSteeringWheelHeatLevelProxy(
        const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(
            autoSteeringWheelHeatLevel_, autoSteeringWheelHeatLevelDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAirQualitySensorProxy(const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(airQualitySensor_,
                                                                                              airQualitySensorDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createManualRecircTimerProxy(const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(manualRecircTimer_,
                                                                                              manualRecircTimerDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createPreconditioningHeatSourceProxy(
        const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;
    return std::make_unique<SettingsProxy<int, UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>>(
            preconditioningHeatSource_, preconditioningHeatSourceDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoFrontDefrosterProxy(const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(autoFrontDefroster_,
                                                                                              autoFrontDefrosterDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAutoRearDefrosterProxy(const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(autoRearDefroster_,
                                                                                              autoRearDefrosterDyno_);
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>
UserSelectionSettingFactory::createAdditionalHeaterProxy(const android::sp<SettingsFramework::SettingsManager>& sm) {
    (void)sm;

    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(additionalHeater_,
                                                                                              additionalHeaterDyno_);
}
}  // namespace setting_factory
}  // namespace user_selection
