/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vfc_driving_climatisation.h"

#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace std::chrono_literals;
using namespace autosar;

namespace {
const std::chrono::seconds timeout{30};
const std::chrono::seconds timeoutMargin{10};
}

VFCDrivingClimatisationLogic::VFCDrivingClimatisationLogic(IDispatcher& timerDispatcher)
    : timerDispatcher_(timerDispatcher), activate_(false) {
    log_debug() << "VFCDrivingClimatisationLogic()";

    VehModMngtGlbSafe1_.subscribe(std::bind(&VFCDrivingClimatisationLogic::handleSignals, this));
    levelOfFanLevelFront_.subscribe(std::bind(&VFCDrivingClimatisationLogic::handleSignals, this));
    seatHeatAndVent_.subscribe(std::bind(&VFCDrivingClimatisationLogic::handleSignals, this));
    seatVentEx_.subscribe(std::bind(&VFCDrivingClimatisationLogic::handleSignals, this));
}

void VFCDrivingClimatisationLogic::handleSignals() {
    activate_ = false;

    if (VehModMngtGlbSafe1_.get().isOk() && (VehModMngtGlbSafe1_.get().value().UsgModSts == UsgModSts1::UsgModDrvg)) {
        if (levelOfFanLevelFront_.get().isOk() && levelOfFanLevelFront_.get().value() != HmiHvacFanLvl::Off) {
            activate_ = true;
        }
        if (seatHeatAndVent_.get().isOk()) {
            if (seatHeatAndVent_.get().value().HmiSeatHeatgForRowFirstLe != SeatClimaLvl::Off) {
                activate_ = true;
            }
            if (seatHeatAndVent_.get().value().HmiSeatHeatgForRowFirstRi != SeatClimaLvl::Off) {
                activate_ = true;
            }
            if (seatHeatAndVent_.get().value().HmiSeatHeatgForRowSecLe != SeatClimaLvl::Off) {
                activate_ = true;
            }
            if (seatHeatAndVent_.get().value().HmiSeatHeatgForRowSecRi != SeatClimaLvl::Off) {
                activate_ = true;
            }
            if (seatHeatAndVent_.get().value().HmiSeatVentnForRowFirstLe != SeatClimaLvl::Off) {
                activate_ = true;
            }
            if (seatHeatAndVent_.get().value().HmiSeatVentnForRowFirstRi != SeatClimaLvl::Off) {
                activate_ = true;
            }
        }
        if (seatVentEx_.get().isOk()) {
            if (seatVentEx_.get().value().HmiSeatVentnForRowSecLe != SeatClimaLvl::Off) {
                activate_ = true;
            }
            if (seatVentEx_.get().value().HmiSeatVentnForRowSecRi != SeatClimaLvl::Off) {
                activate_ = true;
            }
        }
    }

    if (!activate_ && timerDispatcher_.IsRunning()) {
        timerDispatcher_.Cancel();
        request_vfc(Vfc::DrivingClimatisation, 0s);
    } else if (activate_ && !timerDispatcher_.IsRunning()) {
        log_debug() << "request_vfc(Vfc::DrivingClimatisation)";
        request_vfc(Vfc::DrivingClimatisation, timeout + timeoutMargin);

        timerDispatcher_.Start(timeout, [this]() {
            if (activate_) {
                log_verbose() << "request_vfc(Vfc::DrivingClimatisation)";
                request_vfc(Vfc::DrivingClimatisation, timeout + timeoutMargin);
                timerDispatcher_.Restart(timeout);
            }
        });
    }
}
