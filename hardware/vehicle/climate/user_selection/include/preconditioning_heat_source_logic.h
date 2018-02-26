/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "notifiable_property.h"
#include <Application_dataelement.h>
#include <v0/org/volvocars/climate/UserSelection.hpp>

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;

class PreconditioningHeatSourceLogic
{
public:
    PreconditioningHeatSourceLogic(NotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>&);

private:
    NotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>& preconditioningHeatSource_;
    ApplicationDataElement::DESender<autosar::HeatrPreCdngTyp_info>        heatrPreCdngTyp_;

    void               sendSignal();
    SubscriptionHandle subscriptionHandle;
};
