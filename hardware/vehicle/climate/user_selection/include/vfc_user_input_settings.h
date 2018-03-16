/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <Application_dataelement.h>
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "notifiable_property.h"

class VFCUserInputSettingsLogic {
    using UserSelectionGen = v0::org::volvocars::climate::UserSelection;

  public:
    explicit VFCUserInputSettingsLogic(
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& airQualitySensor,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& manualRecircTimer,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoPassengerSeatHeat,
            ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoPassengerSeatHeatLevel,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoDriverSeatHeat,
            ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoDriverSeatHeatLevel,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoSteeringWheelHeat,
            ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoSteeringWheelHeatLevel,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& additionalHeater,
            ReadOnlyNotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>& preconditioningHeatSource,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoFrontDefroster,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoRearDefroster);

  private:
    void setVfc();

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1;

    // First row settings
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& airQualitySensor_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& manualRecircTimer_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoPassengerSeatHeat_;
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoPassengerSeatHeatLevel_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoDriverSeatHeat_;
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoDriverSeatHeatLevel_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoSteeringWheelHeat_;
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoSteeringWheelHeatLevel_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& additionalHeater_;

    ReadOnlyNotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>& preconditioningHeatSource_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoFrontDefroster_;
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoRearDefroster_;

    std::vector<SubscriptionHandle> subscriptionHandles_;
};
