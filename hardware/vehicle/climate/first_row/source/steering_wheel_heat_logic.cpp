/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "steering_wheel_heat_logic.h"

#include "first_row_printers.h"
#include "local_config_helper.h"
#include "logging_context.h"

#include <carconfig.h>
#include <functional>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <v0/org/volvocars/climate/UserSelection.hpp>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using namespace std::chrono_literals;

autosar::SteerWhlHeatgOnCmdTyp SteeringWheelHeatLogic::convertHeatLevelToAutosar(FirstRowGen::HeatLevel level) {
    switch (level) {
        case FirstRowGen::HeatLevel::OFF:
            return autosar::SteerWhlHeatgOnCmdTyp::Off;
        case FirstRowGen::HeatLevel::LO:
            return autosar::SteerWhlHeatgOnCmdTyp::Lo;
        case FirstRowGen::HeatLevel::MED:
            return autosar::SteerWhlHeatgOnCmdTyp::Med;
        case FirstRowGen::HeatLevel::HI:
            return autosar::SteerWhlHeatgOnCmdTyp::Hi;
        default:
            return autosar::SteerWhlHeatgOnCmdTyp::Off;
    }
}

FirstRowGen::HeatLevel const SteeringWheelHeatLogic::convertLevelTypeToHeatLevel(
        UserSelectionGen::LevelType const level) {
    switch (level) {
        case UserSelectionGen::LevelType::LO:
            return FirstRowGen::HeatLevel::LO;
        case UserSelectionGen::LevelType::MED:
            return FirstRowGen::HeatLevel::MED;
        case UserSelectionGen::LevelType::HI:
            return FirstRowGen::HeatLevel::HI;
        default:
            return FirstRowGen::HeatLevel::OFF;
    }
}

bool SteeringWheelHeatLogic::steeringWheelHeatersPresent() {
    return carconfig::getValue<CarConfigParams::CC186_HeatedSteeringWheelType>() ==
           CarConfigParams::CC186_HeatedSteeringWheelType::Heated_steering_wheel;
}

SteeringWheelHeatLogic::SteeringWheelHeatLogic(
        const vcc::LocalConfigReaderInterface* lcfg,
        NotifiableProperty<FirstRowGen::HeatAttribute>& heatAttribute,
        ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoSteeringWheelHeatOn,
        ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoSteeringWheelHeatLevel,
        SettingsProxyInterface<FirstRowGen::HeatLevel::Literal>& steeringWheelHeatLevelSetting,
        tarmac::timeprovider::TimerManagerInterface const& timeProvider)
    : shareHeatAttribute_{heatAttribute},
      setting_{steeringWheelHeatLevelSetting},
      settingGETPORT{setting_.defaultValuePORTHELPER()},
      active_{false},
      timeProvider_{timeProvider},
      timeProviderTimeout_{util::readLocalConfig<std::chrono::seconds>("Climate_Determination_timeout", lcfg)},
      autoSteeringWheelHeatOn_(autoSteeringWheelHeatOn),
      autoSteeringWheelHeatLevel_(autoSteeringWheelHeatLevel) {
    log_debug() << "timeProviderTimeout_ :" << timeProviderTimeout_.count();

    if (steeringWheelHeatersPresent()) {
        // TODO(ARTINFO-503): Callback order on vehicleModeSignal vs setting_(dyno) is undefined,
        //                   old code covered this up by calling Setting.get() which no longer exists
        //                   Need to run full logic in both callbacks.
        vehicleModeSignal_.subscribe([this] { handleVehicleMode(); });
        setting_.subscribe([this](auto newSetting) {
            settingGETPORT = newSetting;
            request(newSetting);
        });
        steeringWheelHeatgAutCdnSignal_.subscribe([this] { handleSteeringWheelHeatAutoSignal(); });
    } else {
        shareHeatAttribute_.set(FirstRowGen::HeatState::NOT_PRESENT, FirstRowGen::HeatLevel::OFF);
    }
}

void SteeringWheelHeatLogic::handleSteeringWheelHeatAutoSignal() {
    if (steeringWheelHeatgAutCdnSignal_.get().value() == autosar::OnOff1::On) {
        log_debug() << "steeringWheelHeatgAutCdnSignal_ is On";

        auto difference = timeProvider_.steady_clock_now() - initialTime_;
        if (difference < timeProviderTimeout_) {
            auto const level =
                    static_cast<UserSelectionGen::LevelType>(autoSteeringWheelHeatLevel_.get().getCurrentLevel());
            log_debug() << "steeringWheelHeatgAutCdnSignal_ callback, "
                           "autoSteeringWheelHeatOn_ is ON, set to auto, "
                        << convertLevelTypeToHeatLevel(level);
            shareHeatAttribute_.set(FirstRowGen::HeatState::AUTO, convertLevelTypeToHeatLevel(level));
            setSteeringWheelHeat(convertHeatLevelToAutosar(convertLevelTypeToHeatLevel(level)));
        } else {
            log_warn() << "Auto condition received after timeout; ignoring...";
        }
    }
}

void SteeringWheelHeatLogic::handleSteeringWheelHeatAutoMode() {
    if (autoSteeringWheelHeatOn_.get().getCurrentSelection() == UserSelectionGen::OffOnType::ON) {
        initialTime_ = timeProvider_.steady_clock_now();
        if (steeringWheelHeatgAutCdnSignal_.get().isOk() &&
            steeringWheelHeatgAutCdnSignal_.get().value() == autosar::OnOff1::On) {
            auto const level =
                    static_cast<UserSelectionGen::LevelType>(autoSteeringWheelHeatLevel_.get().getCurrentLevel());
            log_debug() << "HandleAuto, autoSteeringWheelHeatOn_ is ON, set to auto, "
                        << convertLevelTypeToHeatLevel(level);
            shareHeatAttribute_.set(FirstRowGen::HeatState::AUTO, convertLevelTypeToHeatLevel(level));
            setSteeringWheelHeat(convertHeatLevelToAutosar(convertLevelTypeToHeatLevel(level)));
        } else {
            log_debug() << "HandleAuto, autoSteeringWheelHeatOn_ is ON, set to auto, OFF";
            shareHeatAttribute_.set(FirstRowGen::HeatState::AUTO, FirstRowGen::HeatLevel::OFF);
            setSteeringWheelHeat(autosar::SteerWhlHeatgOnCmdTyp::Off);
        }
    } else {
        log_debug() << "HandleAuto, autoSteeringWhlHeatOn_ is OFF, set to manual, " << settingGETPORT;
        shareHeatAttribute_.set(FirstRowGen::HeatState::MANUAL, settingGETPORT);
        setSteeringWheelHeat(convertHeatLevelToAutosar(settingGETPORT));
    }
}

void SteeringWheelHeatLogic::request(FirstRowGen::HeatLevel const level) {
    if (steeringWheelHeatersPresent() && activationCheckOk()) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        log_debug() << "SteeringWheelHeatLogic request, set to manual, " << level;
        shareHeatAttribute_.set(FirstRowGen::HeatState::MANUAL, level);
        setSteeringWheelHeat(convertHeatLevelToAutosar(level));
        setting_.set(static_cast<FirstRowGen::HeatLevel::Literal>(level.value_));
    } else {
        log_warn() << "SteeringWheelHeatLogic request, when not active";
    }
}

bool SteeringWheelHeatLogic::activationCheckOk() const {
    auto carMode = vehicleModeSignal_.get().value().CarModSts1_;
    auto usageMode = vehicleModeSignal_.get().value().UsgModSts;

    auto carModeStatusOk = (carMode == autosar::CarModSts1::CarModNorm) || (carMode == autosar::CarModSts1::CarModDyno);
    auto usageModeOk = usageMode == autosar::UsgModSts1::UsgModDrvg;

    log_debug() << "SeSteeringWheelHeatLogic activationCheck returns: " << (carModeStatusOk && usageModeOk);

    return carModeStatusOk && usageModeOk;
}

bool SteeringWheelHeatLogic::signalsOk() const {
    return vehicleModeSignal_.get().isOk();
}

void SteeringWheelHeatLogic::handleVehicleMode() {
    if (signalsOk()) {
        if (activationCheckOk()) {
            log_debug() << "SteeringWheelHeatLogic, "
                        << "Car Mode: " << static_cast<int>(vehicleModeSignal_.get().value().CarModSts1_)
                        << ", Usg Mode: " << static_cast<int>(vehicleModeSignal_.get().value().UsgModSts);

            handleSteeringWheelHeatAutoMode();
        } else {
            auto level = static_cast<FirstRowGen::HeatLevel::Literal>(shareHeatAttribute_.get().getHeatLevel().value_);
            setting_.set(level);

            log_debug() << "SteeringWheelHeatLogic, set to DISABLED, off";
            shareHeatAttribute_.set(FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF);
            setSteeringWheelHeat(autosar::SteerWhlHeatgOnCmdTyp::Off);
        }
    } else {
        log_error() << "SteeringWheelHeatLogic, signal in error state";
        shareHeatAttribute_.set(FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF);
        setSteeringWheelHeat(autosar::SteerWhlHeatgOnCmdTyp::Off);
    }
}

void SteeringWheelHeatLogic::setSteeringWheelHeat(autosar::SteerWhlHeatgOnCmdTyp level) {
    steeringWheelHeatSignal_.send(level);
}
