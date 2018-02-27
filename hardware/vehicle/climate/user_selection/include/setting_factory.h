/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "settings_proxy.h"

namespace user_selection {
namespace setting_factory {
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoDriverSeatHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoDriverSeatHeatLevelProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoPassengerSeatHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoPassengerSeatHeatLevelProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoSteeringWheelHeatProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoSteeringWheelHeatLevelProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAirQualitySensorProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createManualRecircTimerProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::NOT_USER_RELATED,
                              SettingsFramework::UserScope::NOT_USER_RELATED>>
createPreconditioningHeatSourceProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoFrontDefrosterProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAutoRearDefrosterProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
createAdditionalHeaterProxy(const android::sp<SettingsFramework::SettingsManager>& sm);
}
}
