/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "setting_factory.h"
#include "v0/org/volvocars/climate/UserSelection.hpp"
#include <libsettings/setting.h>

namespace user_selection
{
namespace setting_factory
{
using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using namespace SettingsFramework;

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoDriverSeatHeatProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoDriverSeatHeat,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF, sm },
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoDriverSeatHeatDyno,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF, sm });
}

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoDriverSeatHeatLevelProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoDriverSeatHeatLevel,
                                                                                 
                                                                                 UserSelectionGen::LevelType::LO, sm },
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoDriverSeatHeatLevelDyno,
                                                                                 
                                                                                 UserSelectionGen::LevelType::LO, sm });
}

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoPassengerSeatHeatProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoPassengerSeatHeat,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF, sm },
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoPassengerSeatHeatDyno,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF , sm});
}

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoPassengerSeatHeatLevelProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoPassengerSeatHeatLevel,
                                                                                 
                                                                                 UserSelectionGen::LevelType::LO, sm },
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoPassengerSeatHeatLevelDyno,
                                                                                 
                                                                                 UserSelectionGen::LevelType::LO, sm });
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoSteeringWheelHeatProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoSteeringWheelHeat,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF, sm },
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoSteeringWheelHeatDyno,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF, sm });
}

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoSteeringWheelHeatLevelProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoSteeringWheelHeatLevel,
                                                                                 
                                                                                 UserSelectionGen::LevelType::LO, sm },
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoSteeringWheelHeatLevelDyno,
                                                                                 
                                                                                 UserSelectionGen::LevelType::LO, sm });
}

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createAirQualitySensorProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>>(
        SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AirQualitySensor,
                                         
                                         static_cast<int>(autosar::OnOff1::On), sm },
        SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AirQualitySensorDyno,
                                         
                                         static_cast<int>(autosar::OnOff1::On) , sm});
}

std::unique_ptr<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>> createManualRecircTimerProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int,UserScope::USER, UserScope::NOT_USER_RELATED>>(
        SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_ManualRecircTimer,
                                         
                                         static_cast<int>(autosar::OnOff1::Off) , sm},
        SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_ManualRecircTimerDyno,
                                         
                                         static_cast<int>(autosar::OnOff1::Off) , sm});
}

std::unique_ptr<SettingsProxy<int, UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>> createPreconditioningHeatSourceProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int, UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED>>(
        SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_PreconditioningHeatSource,
                                         
                                         UserSelectionGen::PreconditioningHeatSourceEnum::AUTO , sm},
        SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_PreconditioningHeatSourceDyno,
                                         
                                         UserSelectionGen::PreconditioningHeatSourceEnum::AUTO , sm});
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoFrontDefrosterProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoFrontDefroster,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF , sm},
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoFrontDefrosterDyno,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF , sm});
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>> createAutoRearDefrosterProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(SettingsFramework::Setting<int, UserScope::USER>{ SettingId::Climate_AutoRearDefroster,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF , sm},
                                                SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{ SettingId::Climate_AutoRearDefrosterDyno,
                                                                                 
                                                                                 UserSelectionGen::OffOnType::OFF , sm});
}

std::unique_ptr<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>> createAdditionalHeaterProxy(android::sp<SettingsFramework::SettingsManager> sm)
{
    return std::make_unique<SettingsProxy<int, UserScope::USER, UserScope::NOT_USER_RELATED>>(
        SettingsFramework::Setting<int, UserScope::USER>{
            SettingId::Climate_AdditionalHeater,  UserSelectionGen::OffOnType::ON , sm},
        SettingsFramework::Setting<int, UserScope::NOT_USER_RELATED>{
            SettingId::Climate_AdditionalHeaterDyno,  UserSelectionGen::OffOnType::ON , sm});
}
}
}
