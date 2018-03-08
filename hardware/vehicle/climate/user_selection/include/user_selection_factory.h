/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <user_selection_stubimpl.hpp>
#include "additional_heater_logic.h"
#include "notifiable_property.h"
#include "preconditioning_heat_source_logic.h"
#include "setting_factory.h"
#include "user_selection_logic.h"
#include "vfc_user_input_settings.h"

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using namespace SettingsFramework;

class UserSelectionFactory final {
  public:
    UserSelectionFactory(const vcc::LocalConfigReaderInterface* lcfg,
                         const android::sp<SettingsFramework::SettingsManager>& settingsManager);
    user_selection::setting_factory::UserSelectionSettingFactory settingsFactory_;
    // Driver Auto Seat Heat
    NotifiableProperty<UserSelectionGen::OffOnSelection> autoDriverSeatHeat_;
    NotifiableProperty<UserSelectionGen::LevelSelection> autoDriverSeatHeatLevel_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> autoDriverSeatHeatLogic_;
    Selection::UserSelectionLogic<UserSelectionGen::LevelSelection> autoDriverSeatHeatLevelLogic_;

    // Passenger Auto Seat Heat
    NotifiableProperty<UserSelectionGen::OffOnSelection> autoPassengerSeatHeat_;
    NotifiableProperty<UserSelectionGen::LevelSelection> autoPassengerSeatHeatLevel_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> autoPassengerSeatHeatLogic_;
    Selection::UserSelectionLogic<UserSelectionGen::LevelSelection> autoPassengerSeatHeatLevelLogic_;

    // Auto Steering Wheel Heat
    NotifiableProperty<UserSelectionGen::OffOnSelection> autoSteeringWheelHeat_;
    NotifiableProperty<UserSelectionGen::LevelSelection> autoSteeringWheelHeatLevel_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> autoSteeringWheelHeatLogic_;
    Selection::UserSelectionLogic<UserSelectionGen::LevelSelection> autoSteeringWheelHeatLevelLogic_;

    // Air Quality
    NotifiableProperty<UserSelectionGen::OffOnSelection> airQualitySensor_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> airQualitySensorLogic_;

    // Recirculation Timer
    NotifiableProperty<UserSelectionGen::OffOnSelection> manualRecircTimer_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> manualRecircTimerLogic_;

    // PreconditioningHeatSource
    NotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct> preconditioningHeatSource_;
    Selection::UserSelectionLogic<UserSelectionGen::PreconditioningHeatSourceStruct> preconditioningHeatSourceLogic_;
    PreconditioningHeatSourceLogic preconditioningHeatSourceSignalLogic_;

    // Auto Front/Rear Defroster
    NotifiableProperty<UserSelectionGen::OffOnSelection> autoFrontDefroster_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> autoFrontDefrosterLogic_;
    NotifiableProperty<UserSelectionGen::OffOnSelection> autoRearDefroster_;
    Selection::UserSelectionLogic<UserSelectionGen::OffOnSelection> autoRearDefrosterLogic_;

    // Additional Heater
    NotifiableProperty<UserSelectionGen::OffOnSelection> additionalHeater_;
    AdditionalHeaterLogic additionalHeaterLogic_;

    VFCUserInputSettingsLogic vfcUserInputSettings_;
};
