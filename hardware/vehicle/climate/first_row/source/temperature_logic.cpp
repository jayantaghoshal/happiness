/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"
#include "temperature_converter.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

TemperatureLogic::TemperatureLogic(bool isValidCarConfig,
                                   common::daemon::TemperatureConverter& tempConverter,
                                   NotifiableProperty<double>& convertedTemp,
                                   NotifiableProperty<double>& storedTemp,
                                   NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedTempHiLoN,
                                   NotifiableProperty<double>& temp,
                                   NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& tempHiLoN,
                                   NotifiableProperty<FirstRowGen::StateType>& state,
                                   ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
                                   ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
                                   ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateResetEvent)
    : tempConverter_(tempConverter),
      convertedTemp_(convertedTemp),
      storedTemp_(storedTemp),
      storedTempHiLoN_(storedTempHiLoN),
      temp_(temp),
      tempHiLoN_(tempHiLoN),
      state_(state),
      fanLevelFront_(fanLevelFront),
      maxDefroster_(maxDefroster),
      climateResetEvent_(climateResetEvent),
      active_(false),
      tUnit_(autosar::AmbTIndcdUnit::Celsius),
      prevMaxDefroster_(maxDefroster.get()),
      mutex_() {
    if (isValidCarConfig) {
        temp_.set(storedTemp.get());
        tempHiLoN_.set(storedTempHiLoN.get());
        convertedTemp_.set(tempConverter_.toSingle(tUnit_, temp_.get(), tempHiLoN_.get()));

        tempId_ = temp_.subscribe([this](const auto&) {
            // We own temp_ so no need to lock mutex
            convertedTemp_.set(tempConverter_.toSingle(tUnit_, temp_.get(), tempHiLoN_.get()));
        });

        tempHiLoNId_ = tempHiLoN_.subscribe([this](const auto&) {
            // We own temp_ so no need to lock mutex
            convertedTemp_.set(tempConverter_.toSingle(tUnit_, temp_.get(), tempHiLoN_.get()));
        });

        storedTempId_ = storedTemp_.subscribe([this](const auto&) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            this->update(storedTemp_.get());
        });
        storedTempHiLoNId_ = storedTempHiLoN_.subscribe([this](const auto&) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            this->update(storedTempHiLoN_.get());
        });

        climateResetEventId_ = climateResetEvent_.subscribe([this](const auto&) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            if (climateResetEvent_.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) {
                temp_.set(22.0);
                tempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);

                // TODO find a better solution for this
                // If maxDefroster is on when climate reset is received, maxDefrosterChanged function
                // might trigger after this function. Set to off to avoid it restoring the temperature
                // from settings. This is a bad hack because we assume maxdefroster will go off.
                prevMaxDefroster_ = FirstRowGen::MaxDefrosterState::Literal::OFF;
            }
        });

        fanLevelFrontId_ = fanLevelFront_.subscribe([this](const auto&) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            this->fanLevelFrontChanged(fanLevelFront_.get());
        });
        maxDefrosterId_ = maxDefroster_.subscribe([this](const auto&) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            this->maxDefrosterChanged(maxDefroster_.get());
        });

        auto handleSignals = [this] {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            if (signalsOk()) {
                auto active = activationCheck(vehModMngtGlbSafe1_.get().value().UsgModSts,
                                              vehModMngtGlbSafe1_.get().value().CarModSts1_,
                                              climateActive_.get().value());

                if (active != active_) {
                    active_ = active;

                    if (active_) {
                        // Reevaluate
                        maxDefrosterChanged(maxDefroster_.get());
                        fanLevelFrontChanged(fanLevelFront_.get());
                    } else {
                        state_.set(FirstRowGen::StateType::DISABLED);
                    }
                }
            } else {
                state_.set(FirstRowGen::StateType::SYSTEM_ERROR);
            }
        };

        vehModMngtGlbSafe1_.subscribe(handleSignals);
        climateActive_.subscribe(handleSignals);
        indcnUnit_.subscribe([this]() {
            std::lock_guard<std::recursive_mutex> lock(mutex_);

            auto newTUnit = indcnUnit_.get().value().TUnit;

            if (newTUnit != tUnit_) {
                tUnit_ = newTUnit;
                convertedTemp_.set(tempConverter_.toSingle(tUnit_, temp_.get(), tempHiLoN_.get()));
            }
        });
    } else {
        state_.set(FirstRowGen::StateType::NOT_PRESENT);
    }
}

TemperatureLogic::~TemperatureLogic() = default;

bool TemperatureLogic::isPassengerCarConfigValid() {
    auto param = carconfig::getValue<CarConfigParams::CC175_HvacVariantsType>();

    switch (param) {
        case CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_2_zone:
        case CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_4_zone:
        case CarConfigParams::CC175_HvacVariantsType::HVAC_Small_2_zone:
            return true;
            break;
        default:
            return false;
            break;
    }
}

void TemperatureLogic::request(double temperature) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    auto const converted = tempConverter_.fromSingle(tUnit_, temperature);
    request(converted.first, converted.second);
}

void TemperatureLogic::request(double temp, autosar::HmiCmptmtTSpSpcl tempHiLoN) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    if (active_) {
        update(temp);
        update(tempHiLoN);
    }
}

FirstRowGen::Range TemperatureLogic::range() {
    auto range = tempConverter_.range(tUnit_);
    return {range.first, range.second};
}

bool TemperatureLogic::signalsOk() const { return (vehModMngtGlbSafe1_.get().isOk() && climateActive_.get().isOk()); }

bool TemperatureLogic::activationCheck(const autosar::UsgModSts1 usgMode,
                                       const autosar::CarModSts1 carMode,
                                       const autosar::OnOff1 parkingClimate) const {
    using namespace autosar;

    auto usageModestatusIsNotAbandoned = usgMode != UsgModSts1::UsgModAbdnd;
    auto usageModestatusIsDriving = usgMode == UsgModSts1::UsgModDrvg;
    auto carModeStatusIsNormalOrDyno = (carMode == CarModSts1::CarModNorm) || (carMode == CarModSts1::CarModDyno);
    auto parkingClimateIsOn = parkingClimate == OnOff1::On;

    auto retVal = (usageModestatusIsDriving || (usageModestatusIsNotAbandoned && parkingClimateIsOn)) &&
                  carModeStatusIsNormalOrDyno;

    return retVal;
}

void TemperatureLogic::update(double temp) {
    log_debug() << "Temp " << temp_.get() << " >> " << temp;
    if (temp != temp_.get()) {
        temp_.set(temp);
    }
}

void TemperatureLogic::update(autosar::HmiCmptmtTSpSpcl tempHiLoN) {
    log_debug() << "TempHiLoN " << static_cast<int>(tempHiLoN_.get()) << " >> " << static_cast<int>(tempHiLoN);
    if (active_ && (tempHiLoN != tempHiLoN_.get())) {
        tempHiLoN_.set(tempHiLoN);
    }
}

void TemperatureLogic::maxDefrosterChanged(const FirstRowGen::MaxDefrosterState& maxDefroster) {
    if (active_) {
        if ((maxDefroster == FirstRowGen::MaxDefrosterState::Literal::ON) &&
            (prevMaxDefroster_ != FirstRowGen::MaxDefrosterState::Literal::ON)) {
            tempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);
        } else if ((maxDefroster != FirstRowGen::MaxDefrosterState::Literal::ON) &&
                   (prevMaxDefroster_ == FirstRowGen::MaxDefrosterState::Literal::ON)) {
            temp_.set(storedTemp_.get());
            tempHiLoN_.set(storedTempHiLoN_.get());
        }
    }

    prevMaxDefroster_ = maxDefroster;
}

void TemperatureLogic::fanLevelFrontChanged(FirstRowGen::FanLevelFrontValue fanLevel) {
    if (active_) {
        if (fanLevel == FirstRowGen::FanLevelFrontValue::OFF) {
            state_.set(FirstRowGen::StateType::DISABLED);
        } else {
            state_.set(FirstRowGen::StateType::AVAILABLE);
        }
    }
}
