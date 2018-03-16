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

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;

class PreconditioningHeatSourceLogic {
  public:
    PreconditioningHeatSourceLogic(NotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>&);

  private:
    NotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>& preconditioningHeatSource_;
    ApplicationDataElement::DESender<autosar::HeatrPreCdngTyp_info> heatrPreCdngTyp_;

    void sendSignal();
    SubscriptionHandle subscriptionHandle;
};
