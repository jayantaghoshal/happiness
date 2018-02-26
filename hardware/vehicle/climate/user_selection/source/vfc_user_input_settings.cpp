/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "vfc_user_input_settings.h"

#include "vfc_helper.h"
#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(UserSelectionContext)

VFCUserInputSettingsLogic::VFCUserInputSettingsLogic(
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  airQualitySensor,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  manualRecircTimer,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  autoPassengerSeatHeat,
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>&                  autoPassengerSeatHeatLevel,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  autoDriverSeatHeat,
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>&                  autoDriverSeatHeatLevel,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  autoSteeringWheelHeat,
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>&                  autoSteeringWheelHeatLevel,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  additionalHeater,
    ReadOnlyNotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>& preconditioningHeatSource,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  autoFrontDefroster,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&                  autoRearDefroster)
    : airQualitySensor_(airQualitySensor)
    , manualRecircTimer_(manualRecircTimer)
    , autoPassengerSeatHeat_(autoPassengerSeatHeat)
    , autoPassengerSeatHeatLevel_(autoPassengerSeatHeatLevel)
    , autoDriverSeatHeat_(autoDriverSeatHeat)
    , autoDriverSeatHeatLevel_(autoDriverSeatHeatLevel)
    , autoSteeringWheelHeat_(autoSteeringWheelHeat)
    , autoSteeringWheelHeatLevel_(autoSteeringWheelHeatLevel)
    , additionalHeater_(additionalHeater)
    , preconditioningHeatSource_(preconditioningHeatSource)
    , autoFrontDefroster_(autoFrontDefroster)
    , autoRearDefroster_(autoRearDefroster)
{
    subscriptionHandles_.push_back(additionalHeater_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(airQualitySensor_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoDriverSeatHeat_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoDriverSeatHeatLevel_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoPassengerSeatHeat_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoPassengerSeatHeatLevel_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoSteeringWheelHeat_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoSteeringWheelHeatLevel_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(manualRecircTimer_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(preconditioningHeatSource_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoFrontDefroster_.subscribe([this](auto) { this->setVfc(); }));
    subscriptionHandles_.push_back(autoRearDefroster_.subscribe([this](auto) { this->setVfc(); }));
}

void VFCUserInputSettingsLogic::setVfc()
{
    if (vehModMngtGlbSafe1.get().isOk()
        && (vehModMngtGlbSafe1.get().value().UsgModSts != autosar::UsgModSts1::UsgModAbdnd))
    {
        log_debug() << "request_vfc(Vfc::UserInputSettings)";
        request_vfc(Vfc::UserInputSettings);
    }
    else
    {
        log_debug() << "VFCUserInputSettings not set, usage mode not valid!";
    }
}
