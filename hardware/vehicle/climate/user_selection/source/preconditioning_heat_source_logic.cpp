/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "preconditioning_heat_source_logic.h"

#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(UserSelectionContext)

namespace {
auto LOG_PREFIX = "PreConditioningHeatSource: ";
}

PreconditioningHeatSourceLogic::PreconditioningHeatSourceLogic(
        NotifiableProperty<UserSelectionGen::PreconditioningHeatSourceStruct>& preconditioningHeatSource)
    : preconditioningHeatSource_(preconditioningHeatSource) {
    subscriptionHandle = preconditioningHeatSource_.subscribe([this](const auto) { this->sendSignal(); });
    sendSignal();
}

void PreconditioningHeatSourceLogic::sendSignal() {
    auto value = autosar::HeatrPreCdngTyp::NoReq;

    if (preconditioningHeatSource_.get().getCurrentState() == UserSelectionGen::StateType::AVAILABLE) {
        switch (preconditioningHeatSource_.get().getCurrentSelection()) {
            case UserSelectionGen::PreconditioningHeatSourceEnum::AUTO:
                value = autosar::HeatrPreCdngTyp::Aut;
                break;
            case UserSelectionGen::PreconditioningHeatSourceEnum::ELECTRIC:
                value = autosar::HeatrPreCdngTyp::Elec;
                break;
            case UserSelectionGen::PreconditioningHeatSourceEnum::FUEL:
                value = autosar::HeatrPreCdngTyp::Fu;
                break;
            default:
                break;
        }
    }

    heatrPreCdngTyp_.send(value);

    log_debug() << LOG_PREFIX << "state : " << preconditioningHeatSource_.get().getCurrentState()
                << ", selection: " << preconditioningHeatSource_.get().getCurrentSelection();
}
