/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_reset_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

ClimateResetLogic::ClimateResetLogic(NotifiableProperty<ClimateResetEvent>& event)
    : shareClimateResetEvent{event}, active_{false}, carConfigOk_{false} {
    carConfigOk_ = isCarConfigValid();
    if (carConfigOk_) {
        parkingClimate.subscribe([this]() { handleParkingClimate(); });
        vehicleModes.subscribe([this]() { handleVehicleMode(); });
        activationCheck();
    }
}

void ClimateResetLogic::request() {
    if (carConfigOk_ && active_) {
        shareClimateResetEvent.set(ClimateResetEvent::ACTIVATED);
        shareClimateResetEvent.set(ClimateResetEvent::NO_REQ);
    }
}

void ClimateResetLogic::handleParkingClimate() {
    std::lock_guard<Mutex> lock(mutex_);

    if (parkingClimate.get().isOk()) {
        parkingClimateActive_ = (parkingClimate.get().value() == autosar::OnOff1::On);
        activationCheck();
    }
}

void ClimateResetLogic::handleVehicleMode() {
    std::lock_guard<Mutex> lock(mutex_);

    if (vehicleModes.get().isOk()) {
        usageMode_ = vehicleModes.get().value().UsgModSts;
        carMode_ = vehicleModes.get().value().CarModSts1_;
        activationCheck();
    }
}

void ClimateResetLogic::activationCheck() {
    if ((usageMode_ == autosar::UsgModSts1::UsgModDrvg ||
         (parkingClimateActive_ && usageMode_ != autosar::UsgModSts1::UsgModAbdnd)) &&
        (carMode_ == autosar::CarModSts1::CarModNorm || carMode_ == autosar::CarModSts1::CarModDyno)) {
        active_ = true;
    } else {
        active_ = false;
    }
}

bool ClimateResetLogic::isCarConfigValid() const {
    using HvacVariants = CarConfigParams::CC175_HvacVariantsType;

    try {
        const auto HvacVariant = carconfig::getValue<HvacVariants>();

        return (HvacVariant == HvacVariants::Electronic_Climate_Control_2_zone ||
                HvacVariant == HvacVariants::Electronic_Climate_Control_4_zone ||
                HvacVariant == HvacVariants::HVAC_Small_1_zone || HvacVariant == HvacVariants::HVAC_Small_2_zone);
    } catch (const std::exception& ia) {
        log_error() << "ClimateResetLogic::isCarConfigValid: Car config was throwing an exeption: " +
                               std::string(ia.what());
        return false;
    }
}
