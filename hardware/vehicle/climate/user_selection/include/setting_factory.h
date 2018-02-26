/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include "settings_proxy.h"

namespace user_selection
{
namespace setting_factory
{
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoDriverSeatHeatProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoDriverSeatHeatLevelProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoPassengerSeatHeatProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoPassengerSeatHeatLevelProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoSteeringWheelHeatProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoSteeringWheelHeatLevelProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAirQualitySensorProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createManualRecircTimerProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::NOT_USER_RELATED, SettingsFramework::UserScope::NOT_USER_RELATED>> createPreconditioningHeatSourceProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoFrontDefrosterProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAutoRearDefrosterProxy(android::sp<SettingsFramework::SettingsManager> sm);
std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>> createAdditionalHeaterProxy(android::sp<SettingsFramework::SettingsManager> sm);

}
}
