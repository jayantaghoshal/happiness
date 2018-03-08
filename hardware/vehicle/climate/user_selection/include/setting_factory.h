/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "settings_proxy.h"

namespace user_selection {
namespace setting_factory {

class UserSelectionSettingFactory {
  public:
    UserSelectionSettingFactory(const android::sp<SettingsFramework::SettingsManager>& sm);

    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoDriverSeatHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoDriverSeatHeatLevelProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoPassengerSeatHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoPassengerSeatHeatLevelProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoSteeringWheelHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoSteeringWheelHeatLevelProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAirQualitySensorProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createManualRecircTimerProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::NOT_USER_RELATED,
                                  SettingsFramework::UserScope::NOT_USER_RELATED>>
    createPreconditioningHeatSourceProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoFrontDefrosterProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAutoRearDefrosterProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
    createAdditionalHeaterProxy(const android::sp<SettingsFramework::SettingsManager>& sm);

  private:
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> autoDriverSeatHeat_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> autoDriverSeatHeatDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> autoDriverSeatHeatLevel_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> autoDriverSeatHeatLevelDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> passengerSeatHeat_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> passengerSeatHeatDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> passengerSeatHeatLevel_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> passengerSeatHeatLevelDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> autoSteeringWheelHeat_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> autoSteeringWheelHeatDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> autoSteeringWheelHeatLevel_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> autoSteeringWheelHeatLevelDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> airQualitySensor_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> airQualitySensorDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> manualRecircTimer_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> manualRecircTimerDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> preconditioningHeatSource_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> preconditioningHeatSourceDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> autoFrontDefroster_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> autoFrontDefrosterDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> autoRearDefroster_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> autoRearDefrosterDyno_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::USER> additionalHeater_;
    SettingsFramework::Setting<int, SettingsFramework::UserScope::NOT_USER_RELATED> additionalHeaterDyno_;
};
}
}
