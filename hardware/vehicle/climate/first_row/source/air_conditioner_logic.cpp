/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "air_conditioner_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace SettingsFramework;
using namespace autosar;

AirConditionerLogic::AirConditionerLogic(
        SettingsProxy<FirstRowGen::AirConditionerState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>&
                sAirConditioner,
        NotifiableProperty<FirstRowGen::AirConditionerState>& airConditioner,
        ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
        ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
        ReadOnlyNotifiableProperty<AutoClimateLogic::AutoClimateEvent>& autoClimateEvent,
        ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset)
    : settingAirConditioner(sAirConditioner),
      settingAirConditionerGETPORT{settingAirConditioner.defaultValuePORTHELPER()},
      shareAirConditioner(airConditioner),
      shareFanLevelFront(fanLevelFront),
      shareMaxDefroster(maxDefroster),
      shareAutoClimateEvent(autoClimateEvent),
      shareClimateReset(climateReset),
      inCarModeDyno_(false),
      isActive_(false) {
    auto handleSignals = [this] {
        if (signalsOk()) {
            auto isActive = activationCheck(VehModMngtGlbSafe1_.get().value().UsgModSts,
                                            VehModMngtGlbSafe1_.get().value().CarModSts1_,
                                            climateActiveSignal_.get().value());

            if (isActive != isActive_) {
                isActive_ = isActive;
                setState(isActive_, shareFanLevelFront.get(), settingAirConditionerGETPORT);
            }
        } else {
            isActive_ = false;
            setState(isActive_, shareFanLevelFront.get(), settingAirConditionerGETPORT, true);

            log_warning() << "AirConditionerLogic, signal(s) in error state";
        }
    };

    fanLevelFrontId_ = shareFanLevelFront.subscribe(
            [this](const auto&) { this->setState(isActive_, shareFanLevelFront.get(), settingAirConditionerGETPORT); });

    maxDefrosterId_ = shareMaxDefroster.subscribe([this](const auto&) {
        if (shareMaxDefroster.get() == FirstRowGen::MaxDefrosterState::ON) {
            log_debug() << "AirConditionerLogic Max defroster ON";
            this->setState(isActive_, shareFanLevelFront.get(), FirstRowGen::AirConditionerState::AUTO);
        } else {
            this->setState(isActive_, shareFanLevelFront.get(), settingAirConditionerGETPORT);
        }
    });

    autoClimateEventId_ = shareAutoClimateEvent.subscribe([this](const auto&) {
        if (shareAutoClimateEvent.get() == AutoClimateLogic::AutoClimateEvent::ACTIVATED) {
            log_debug() << "AirConditionerLogic Auto Climate ACTIVATED, request AC to AUTO";
            this->requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);
        }
    });

    climateResetId_ = shareClimateReset.subscribe([this](const auto&) {
        if (shareClimateReset.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) {
            log_debug() << "AirConditionerLogic: Handling climate reset";
            this->requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);
        }
    });

    // IN signals
    climateActiveSignal_.subscribe(handleSignals);
    VehModMngtGlbSafe1_.subscribe(handleSignals);

    settingAirConditioner.subscribe([this](auto newSetting) {
        settingAirConditionerGETPORT = newSetting;
        this->setState(isActive_, shareFanLevelFront.get(), settingAirConditionerGETPORT);
    });

    // Initialization
    handleSignals();
}

AirConditionerLogic::~AirConditionerLogic() {
    std::lock_guard<std::mutex> locker(mutex_);

    settingAirConditioner.unSubscribe();
}

void AirConditionerLogic::requestAirConditioner(FirstRowGen::AirConditionerRequest value) {
    std::lock_guard<std::mutex> locker(mutex_);

    if (value == FirstRowGen::AirConditionerRequest::AUTO) {
        settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
        log_debug() << "AirConditionerLogic: request AC AUTO";
    } else if (value == FirstRowGen::AirConditionerRequest::OFF) {
        settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
        log_debug() << "AirConditionerLogic: request AC OFF";
    } else {
        log_error() << "AirConditionerLogic: AC request DISABLE not a valid request";
    }

    setState(isActive_, shareFanLevelFront.get(), settingAirConditionerGETPORT);
}

void AirConditionerLogic::setState(bool isActive,
                                   const FirstRowGen::FanLevelFrontValue fanLevel,
                                   FirstRowGen::AirConditionerState airConditioner,
                                   bool isInError) {
    log_debug() << "AirConditionerLogic, isActive: " << isActive << ", isInError: " << isInError
                << ", fanLevel: " << fanLevel << ", airConditioner: " << airConditioner;

    if (isInError) {
        shareAirConditioner.set(FirstRowGen::AirConditionerState::SYSTEM_ERROR);
    } else {
        if (isActive && (fanLevel != FirstRowGen::FanLevelFrontValue::SYSTEM_ERROR) &&
            (fanLevel != FirstRowGen::FanLevelFrontValue::DISABLED)) {
            if (fanLevel == FirstRowGen::FanLevelFrontValue::OFF) {
                shareAirConditioner.set(FirstRowGen::AirConditionerState::DISABLED);
            } else {
                if (airConditioner == FirstRowGen::AirConditionerState::OFF) {
                    shareAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
                } else if (airConditioner == FirstRowGen::AirConditionerState::AUTO) {
                    shareAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
                } else {
                    shareAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
                }
            }
        } else {
            shareAirConditioner.set(FirstRowGen::AirConditionerState::DISABLED);
        }
    }

    sendAcSignal();
}

void AirConditionerLogic::sendAcSignal() {
    if (shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO) {
        hmiCmptmtCoolgReq_.send(HmiCmptmtCoolgReq::Auto);
    } else {
        hmiCmptmtCoolgReq_.send(HmiCmptmtCoolgReq::Off);
    }
}

bool AirConditionerLogic::signalsOk() const {
    return (VehModMngtGlbSafe1_.get().isOk() && climateActiveSignal_.get().isOk());
}

bool AirConditionerLogic::activationCheck(const UsgModSts1 usgMode,
                                          const CarModSts1 carMode,
                                          const OnOff1 parkingClimate) const {
    auto usageModestatusIsNotAbandoned = (usgMode != UsgModSts1::UsgModAbdnd);
    auto usageModestatusIsDriving = (usgMode == UsgModSts1::UsgModDrvg);
    auto carModeStatusIsNormal = (carMode == CarModSts1::CarModNorm);
    auto carModeStatusIsDyno = (carMode == CarModSts1::CarModDyno);
    auto parkingClimateIsOn = (parkingClimate == OnOff1::On);

    // Not abandoned with parking climate on or we are driving
    // AND car is in normal or dyno mode
    return (((usageModestatusIsNotAbandoned && parkingClimateIsOn) || usageModestatusIsDriving) &&
            (carModeStatusIsNormal || carModeStatusIsDyno));
}
