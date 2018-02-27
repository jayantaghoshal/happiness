/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "max_defroster_logic.h"

#include "first_row_printers.h"
#include "kpi_log.h"
#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

MaxDefrosterLogic::MaxDefrosterLogic(NotifiableProperty<FirstRowGen::MaxDefrosterState>& state,
                                     ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFrontState,
                                     ReadOnlyNotifiableProperty<FirstRowGen::AirDistributionAngle>& airDistState,
                                     ReadOnlyNotifiableProperty<FirstRowGen::AutoClimateState>& autoClimate,
                                     ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset)
    : shareMaxDefroster{state},
      shareFanLevelFront{fanLevelFrontState},
      shareAirDistribution{airDistState},
      shareAutoClimate{autoClimate},
      shareClimateReset{climateReset},
      lastCarMode(static_cast<autosar::CarModSts1>(-1)),
      lastUsgMode(static_cast<autosar::UsgModSts1>(-1)),
      lastClimaActive(static_cast<autosar::OnOff1>(-1)),
      isActive{false} {
    maxDefrosterId = shareMaxDefroster.subscribe([this](const auto&) {
        switch (shareMaxDefroster.get()) {
            case FirstRowGen::MaxDefrosterState::ON:
                maxDefrosterSignal.send(autosar::ActrReq::On);
                log_info() << KPI_MARKER << "Max Defroster signal ON";
                break;
            case FirstRowGen::MaxDefrosterState::OFF:
            case FirstRowGen::MaxDefrosterState::DISABLED:
                maxDefrosterSignal.send(autosar::ActrReq::Off);
                log_info() << KPI_MARKER << "Max Defroster signal OFF";
                break;
        }
    });

    vehicleModeSignal.subscribe([this] { handleSignals(); });
    climaActiveSignal.subscribe([this] { handleSignals(); });

    fanLevelFrontId = shareFanLevelFront.subscribe([this](const auto&) {
        if (shareFanLevelFront.get() == FirstRowGen::FanLevelFrontValue::OFF ||
            shareFanLevelFront.get() == FirstRowGen::FanLevelFrontValue::DISABLED ||
            shareFanLevelFront.get() == FirstRowGen::FanLevelFrontValue::SYSTEM_ERROR) {
            log_debug() << "MaxDefroster: FanLevel turned off; turning of Max Defroster as well";
            this->request(FirstRowGen::MaxDefrosterRequest::OFF);
        }
    });

    airDistributionId = shareAirDistribution.subscribe([this](const auto&) {
        if (shareAirDistribution.get() != FirstRowGen::AirDistributionAngle::AUTO) {
            log_debug() << "MaxDefroster: AirDistribution no longer AUTO; turning of Max Defroster";
            this->request(FirstRowGen::MaxDefrosterRequest::OFF);
        }
    });

    autoClimateId = shareAutoClimate.subscribe([this](const auto&) {
        if (shareAutoClimate.get() == FirstRowGen::AutoClimateState::ON) {
            log_debug() << "MaxDefroster: Auto climate ON; turning of Max Defroster";
            this->request(FirstRowGen::MaxDefrosterRequest::OFF);
        }
    });

    climateResetId = shareClimateReset.subscribe([this](const auto&) {
        if (shareClimateReset.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) {
            log_debug() << "MaxDefrosterLogic: Handling climate reset";
            this->request(FirstRowGen::MaxDefrosterRequest::OFF);
        }
    });
}

void MaxDefrosterLogic::request(FirstRowGen::MaxDefrosterRequest state) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    if (isActive) {
        switch (state) {
            case FirstRowGen::MaxDefrosterRequest::OFF:
                shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
                break;
            case FirstRowGen::MaxDefrosterRequest::ON:
                shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);
                break;
            default:
                break;
        }
    }
}

void MaxDefrosterLogic::request(OnOff requestedState) {
    log_debug() << "MaxDefroster, requestCCSM " << requestedState;

    if (requestedState == OnOff::On) {
        request(FirstRowGen::MaxDefrosterRequest::ON);
    } else {
        request(FirstRowGen::MaxDefrosterRequest::OFF);
    }
}

void MaxDefrosterLogic::handleSignals() {
    std::lock_guard<std::recursive_mutex> lock(mutex);

    if (signalsOk()) {
        isActive = activationCheck(vehicleModeSignal.get().value().UsgModSts,
                                   vehicleModeSignal.get().value().CarModSts1_, climaActiveSignal.get().value());
        if (isActive) {
            if ((vehicleModeSignal.get().value().CarModSts1_ != lastCarMode) ||
                vehicleModeSignal.get().value().UsgModSts != lastUsgMode ||
                climaActiveSignal.get().value() != lastClimaActive) {
                shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
            }
        } else {
            shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::DISABLED);
        }

        lastCarMode = vehicleModeSignal.get().value().CarModSts1_;
        lastUsgMode = vehicleModeSignal.get().value().UsgModSts;
        lastClimaActive = climaActiveSignal.get().value();
    }
}

bool MaxDefrosterLogic::activationCheck(autosar::UsgModSts1 const usgModeSts1, autosar::CarModSts1 const carModSts1,
                                        autosar::OnOff1 const climaActive) const {
    auto usgModeIsDriving = usgModeSts1 == autosar::UsgModSts1::UsgModDrvg;
    auto carModeIsNormal = carModSts1 == autosar::CarModSts1::CarModNorm;
    auto carModeIsDyno = carModSts1 == autosar::CarModSts1::CarModDyno;
    auto climaActiveIsOn = climaActive == autosar::OnOff1::On;

    return ((usgModeIsDriving || climaActiveIsOn) && (carModeIsDyno || carModeIsNormal));
}

bool MaxDefrosterLogic::signalsOk() const { return (vehicleModeSignal.get().isOk() && climaActiveSignal.get().isOk()); }
