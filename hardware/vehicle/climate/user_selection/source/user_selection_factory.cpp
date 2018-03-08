/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "user_selection_factory.h"

#include "kpi_log.h"
#include "logging_context.h"
#include "setting_factory.h"
#include "user_selection_stubimpl.hpp"
#include "vfc_helper.h"

#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(UserSelectionContext)

using namespace autosar;
using namespace user_selection;

UserSelectionFactory::UserSelectionFactory(const vcc::LocalConfigReaderInterface* lcfg,
                                           const android::sp<SettingsFramework::SettingsManager>& settingsManager)
    : settingsFactory_{settingsManager},
      autoDriverSeatHeat_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      autoDriverSeatHeatLevel_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::LevelType::LO}},
      autoDriverSeatHeatLogic_{autoDriverSeatHeat_, settingsFactory_.createAutoDriverSeatHeatProxy(settingsManager),
                               CarConfigParams::CC180_HeatedFrontSeatsType::Heated_front_seats},
      autoDriverSeatHeatLevelLogic_{autoDriverSeatHeatLevel_,
                                    settingsFactory_.createAutoDriverSeatHeatLevelProxy(settingsManager),
                                    CarConfigParams::CC180_HeatedFrontSeatsType::Heated_front_seats},
      autoPassengerSeatHeat_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      autoPassengerSeatHeatLevel_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::LevelType::LO}},
      autoPassengerSeatHeatLogic_{autoPassengerSeatHeat_,
                                  settingsFactory_.createAutoPassengerSeatHeatProxy(settingsManager),
                                  CarConfigParams::CC180_HeatedFrontSeatsType::Heated_front_seats},
      autoPassengerSeatHeatLevelLogic_{autoPassengerSeatHeatLevel_,
                                       settingsFactory_.createAutoPassengerSeatHeatLevelProxy(settingsManager),
                                       CarConfigParams::CC180_HeatedFrontSeatsType::Heated_front_seats},
      autoSteeringWheelHeat_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      autoSteeringWheelHeatLevel_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::LevelType::LO}},
      autoSteeringWheelHeatLogic_{autoSteeringWheelHeat_,
                                  settingsFactory_.createAutoSteeringWheelHeatProxy(settingsManager),
                                  CarConfigParams::CC186_HeatedSteeringWheelType::Heated_steering_wheel},
      autoSteeringWheelHeatLevelLogic_{autoSteeringWheelHeatLevel_,
                                       settingsFactory_.createAutoSteeringWheelHeatLevelProxy(settingsManager),
                                       CarConfigParams::CC186_HeatedSteeringWheelType::Heated_steering_wheel},
      airQualitySensor_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      airQualitySensorLogic_{airQualitySensor_, settingsFactory_.createAirQualitySensorProxy(settingsManager),
                             CarConfigParams::CC174_AirQualitySystemType::Air_Quality_System},
      manualRecircTimer_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      manualRecircTimerLogic_{manualRecircTimer_, settingsFactory_.createManualRecircTimerProxy(settingsManager)},
      preconditioningHeatSource_{
              {UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::PreconditioningHeatSourceEnum::AUTO}},
      preconditioningHeatSourceLogic_{preconditioningHeatSource_,
                                      settingsFactory_.createPreconditioningHeatSourceProxy(settingsManager),
                                      CarConfigParams::CC182_ParkingClimateLevelType::Full_parking_climate,
                                      CarConfigParams::CC347_HeaterTypeType::FOH_Fuel_Operated_Heater,
                                      CarConfigParams::CC355_PtcInFrontHvacType::With_PTC_1000W},
      preconditioningHeatSourceSignalLogic_{preconditioningHeatSource_},
      autoFrontDefroster_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      autoFrontDefrosterLogic_{autoFrontDefroster_, settingsFactory_.createAutoFrontDefrosterProxy(settingsManager),
                               CarConfigParams::CC122_HeatedWindscreenType::Heated_Frontscreen},
      autoRearDefroster_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      autoRearDefrosterLogic_{autoRearDefroster_, settingsFactory_.createAutoRearDefrosterProxy(settingsManager)},
      additionalHeater_{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
      additionalHeaterLogic_{lcfg, additionalHeater_, settingsFactory_.createAdditionalHeaterProxy(settingsManager)},
      vfcUserInputSettings_{airQualitySensor_,           manualRecircTimer_,          autoPassengerSeatHeat_,
                            autoPassengerSeatHeatLevel_, autoDriverSeatHeat_,         autoDriverSeatHeatLevel_,
                            autoSteeringWheelHeat_,      autoSteeringWheelHeatLevel_, additionalHeater_,
                            preconditioningHeatSource_,  autoFrontDefroster_,         autoRearDefroster_}
// TODO: Add ThirdRowClimate to vfcUserInputSettings
{
    // TODO(climateport) Common API setup removed

    auto userSelectionService = std::make_shared<v0::org::volvocars::climate::UserSelectionStubImpl>(*this);
    userSelectionService->initStubAdapter();
}
