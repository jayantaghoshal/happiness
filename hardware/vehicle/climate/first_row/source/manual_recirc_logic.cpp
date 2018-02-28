/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "manual_recirc_logic.h"

#include "autosar_printers.h"
#include "first_row_printers.h"
#include "kpi_log.h"
#include "local_config_helper.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>
#include <functional>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace SettingsFramework;
using namespace autosar;
using namespace std::chrono_literals;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

namespace {

bool carConfigState() {
    bool result = false;

    try {
        const auto airQualitySystem = carconfig::getValue<CarConfigParams::CC174_AirQualitySystemType>();

        result = airQualitySystem == CarConfigParams::CC174_AirQualitySystemType::Air_Quality_System;
    } catch (const std::out_of_range& ia) {
        log_error() << "Car config threw: " + std::string(ia.what());
    }

    return result;
}
}  // namespace

ManualRecircLogic::ManualRecircLogic(ReadOnlyNotifiableProperty<userSelectionGen::OffOnSelection>& RecircTimer,
                                     ReadOnlyNotifiableProperty<userSelectionGen::OffOnSelection>& AirQualitySensor,
                                     SettingsProxy<FirstRowGen::ManualRecircState::Literal, UserScope::USER,
                                                   UserScope::NOT_USER_RELATED>& settingManualRecirc,
                                     NotifiableProperty<FirstRowGen::ManualRecircState>& sharedManualRecirc,
                                     ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset,
                                     ILegacyDispatcher& timerDispatcher,
                                     ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
                                     ReadOnlyNotifiableProperty<AutoClimateLogic::AutoClimateEvent>& autoClimate,
                                     ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster)
    : shareClimateReset{climateReset},
      shareMaxDefroster{maxDefroster},
      shareFanLevelFront{fanLevelFront},
      shareAutoClimate{autoClimate},
      shareRecircTimer{RecircTimer},
      shareAirQualitySensor{AirQualitySensor},
      shareMaxDefrosterId{},
      shareFanLevelFrontId{},
      shareAutoClimateId{},
      shareAirQualitySensorId{},
      sManualRecircProxy{settingManualRecirc},
      sharedManualRecirc_{sharedManualRecirc},
      isActive_{false},
      carConfig_{false},
      timerDispatcher_{timerDispatcher},
      timeout_{util::readLocalConfig<std::chrono::minutes>("Climate_Manual_Recirc")} {
    carConfig_ = carConfigState();

    auto handleSignals = [this] {
        if (signalsOk()) {
            auto isActive = activationCheck(vehModSts_.get().value().UsgModSts, vehModSts_.get().value().CarModSts1_,
                                            climateActiveSignal_.get().value());

            if ((isActive_ != isActive)) {
                isActive_ = isActive;
                this->setStateAndValue(isActive_, shareFanLevelFront.get(), shareMaxDefroster.get());
            }
        } else {
            isActive_ = false;
            sharedManualRecirc_.set(FirstRowGen::ManualRecircState::SYSTEM_ERROR);
        }
    };

    // Init all states and values
    handleSignals();

    sManualRecircProxy.subscribe(
            [this]() { setStateAndValue(isActive_, shareFanLevelFront.get(), shareMaxDefroster.get()); });

    shareAirQualitySensorId = shareAirQualitySensor.subscribe([this](const auto) {
        this->setStateAndValue(isActive_, shareFanLevelFront.get(), shareMaxDefroster.get());
    });

    shareMaxDefrosterId = shareMaxDefroster.subscribe([this](const auto) {
        this->setStateAndValue(isActive_, shareFanLevelFront.get(), shareMaxDefroster.get());
    });

    shareFanLevelFrontId = shareFanLevelFront.subscribe([this](const auto) {
        this->setStateAndValue(isActive_, shareFanLevelFront.get(), shareMaxDefroster.get());
    });

    shareAutoClimateId = shareAutoClimate.subscribe([this](const auto) {
        if (shareAutoClimate.get() == AutoClimateLogic::AutoClimateEvent::ACTIVATED) {
            this->requestManualRecirc(FirstRowGen::ManualRecircRequest::OFF);
        }
    });

    shareClimateResetId = shareClimateReset.subscribe([this](const auto) {
        if (shareClimateReset.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) {
            log_debug() << "ManualRecircLogic: Handling climate reset";
            this->requestManualRecirc(FirstRowGen::ManualRecircRequest::OFF);
        }
    });

    // IN signals
    climateActiveSignal_.subscribe(handleSignals);
    vehModSts_.subscribe(handleSignals);
}

void ManualRecircLogic::requestManualRecirc(FirstRowGen::ManualRecircRequest value) {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    switch (value) {
        case FirstRowGen::ManualRecircRequest::ON:
            log_debug() << "ManualRecircLogic: Requested On";
            sManualRecircProxy.set(
                    static_cast<FirstRowGen::ManualRecircState::Literal>(FirstRowGen::ManualRecircState::ON));
            break;
        case FirstRowGen::ManualRecircRequest::OFF:
            log_debug() << "ManualRecircLogic: Requested Off";
            sManualRecircProxy.set(
                    static_cast<FirstRowGen::ManualRecircState::Literal>(FirstRowGen::ManualRecircState::OFF));
            break;
        default:
            log_error() << "ManualRecircLogic: Unknown recirc request";
            break;
    }
}

void ManualRecircLogic::setStateAndValue(bool isActive, FirstRowGen::FanLevelFrontValue fanLevel,
                                         FirstRowGen::MaxDefrosterState maxDefroster) {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    log_debug() << "ManualRecircLogic, isActive: " << isActive << ", fanLevel: " << fanLevel
                << ", maxDefroster: " << maxDefroster;

    if (isActive) {
        if ((fanLevel == FirstRowGen::FanLevelFrontValue::OFF) ||
            (maxDefroster == FirstRowGen::MaxDefrosterState::ON)) {
            sharedManualRecirc_.set(FirstRowGen::ManualRecircState::DISABLED);
        } else {
            auto currentManualRecirc = FirstRowGen::ManualRecircState::OFF;

            currentManualRecirc = sManualRecircProxy.get();

            switch (currentManualRecirc) {
                case FirstRowGen::ManualRecircState::ON:
                    sharedManualRecirc_.set(FirstRowGen::ManualRecircState::ON);

                    if ((shareRecircTimer.get().getCurrentState() == userSelectionGen::StateType::AVAILABLE) &&
                        (shareRecircTimer.get().getCurrentSelection() == userSelectionGen::OffOnType::ON)) {
                        log_debug() << "ManualRecircLogic: Timer started " << timeout_.count() << "minutes";
                        timerDispatcher_.Start(
                                timeout_, [this]() { requestManualRecirc(FirstRowGen::ManualRecircRequest::OFF); });
                    }
                    break;
                case FirstRowGen::ManualRecircState::OFF:
                    sharedManualRecirc_.set(FirstRowGen::ManualRecircState::OFF);
                    break;
                default:
                    log_error() << "Should not end up here!";
                    break;
            }
        }
    } else {
        sharedManualRecirc_.set(FirstRowGen::ManualRecircState::DISABLED);
    }

    log_debug() << "ManualRecircLogic, State: " << sharedManualRecirc_.get();

    sendSignal();
}

void ManualRecircLogic::sendSignal() {
    if (sharedManualRecirc_.get() == FirstRowGen::ManualRecircState::ON) {
        hmiHvacRecircCmd_.send(HmiHvacRecircCmd::RecircFull);
    } else {
        if ((carConfig_) && (shareAirQualitySensor.get().getCurrentState() == userSelectionGen::StateType::AVAILABLE) &&
            (shareAirQualitySensor.get().getCurrentSelection() == userSelectionGen::OffOnType::ON)) {
            hmiHvacRecircCmd_.send(HmiHvacRecircCmd::AutWithAirQly);
        } else {
            hmiHvacRecircCmd_.send(HmiHvacRecircCmd::Aut);
        }
    }
}

bool ManualRecircLogic::signalsOk() { return vehModSts_.get().isOk() && climateActiveSignal_.get().isOk(); }

bool ManualRecircLogic::activationCheck(const UsgModSts1 usgMode, const CarModSts1 carMode,
                                        const OnOff1 parkingClimate) {
    auto carModeStatusIsNormal = (carMode == CarModSts1::CarModNorm);
    auto carModeStatusIsDyno = (carMode == CarModSts1::CarModDyno);
    auto parkingClimateIsOn = (parkingClimate == OnOff1::On);
    auto usageModestatusIsDriving = (usgMode == UsgModSts1::UsgModDrvg);

    log_debug() << "ManualRecircLogic, usgMode: " << usgMode << ", carMode: " << carMode
                << ", parkingClimate: " << static_cast<int>(parkingClimate);

    auto result = (carModeStatusIsNormal || carModeStatusIsDyno) && (parkingClimateIsOn || usageModestatusIsDriving);

    log_info() << KPI_MARKER << "ManualRecirc active, " << (result ? "true" : "false");

    return result;
}
