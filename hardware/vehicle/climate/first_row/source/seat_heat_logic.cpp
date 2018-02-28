/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "seat_heat_logic.h"

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

namespace {
autosar::SeatClimaLvl toAutosar(FirstRowGen::HeatLevel level) {
    switch (level) {
        case v0::org::volvocars::climate::FirstRow::HeatLevel::OFF:
            return autosar::SeatClimaLvl::Off;
        case v0::org::volvocars::climate::FirstRow::HeatLevel::LO:
            return autosar::SeatClimaLvl::Lvl1;
        case v0::org::volvocars::climate::FirstRow::HeatLevel::MED:
            return autosar::SeatClimaLvl::Lvl2;
        case v0::org::volvocars::climate::FirstRow::HeatLevel::HI:
            return autosar::SeatClimaLvl::Lvl3;
        default:
            return autosar::SeatClimaLvl::Off;
    }
}

FirstRowGen::HeatLevel const toHeatLevel(UserSelectionGen::LevelType const level) {
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

bool seatHeatersPresent() {
    try {
        auto result = carconfig::getValue<CarConfigParams::CC180_HeatedFrontSeatsType>();
        return result == CarConfigParams::CC180_HeatedFrontSeatsType::Heated_front_seats;
    } catch (std::out_of_range const& exception) {
        log_fatal() << "CarConfig threw an exception: " << exception.what();
        throw;
    }
}
}  // namespace

SeatHeatLogic::SeatHeatLogic(UserLocation userLocation, autosar::HmiSeatClima& seatClimate,
                             NotifiableProperty<FirstRowGen::HeatAttribute>& heatAttribute,
                             NotifiableProperty<UserSelectionGen::OffOnSelection>& autoSeatHeatOn,
                             NotifiableProperty<UserSelectionGen::LevelSelection>& autoSeatHeatLevel,
                             SettingsProxyInterface<FirstRowGen::HeatLevel::Literal>& seatHeatLevelSetting,
                             tarmac::timeprovider::TimerManagerInterface const& timeProvider)
    : userLocation_{userLocation},
      shareHeatAttribute_{heatAttribute},
      sHeatLevel_{seatHeatLevelSetting},
      active_{false},
      seatClimate_{seatClimate},
      seatHeatSignal_{getSeatHeatSignal()},
      timeProvider_{timeProvider},
      timeProviderTimeout_{util::readLocalConfig<std::chrono::seconds>("Climate_Determination_timeout")},
      autoSeatHeatOn_(autoSeatHeatOn),
      autoSeatHeatLevel_(autoSeatHeatLevel) {
    log_debug() << "timeProviderTimeout_ :" << timeProviderTimeout_.count();

    if (seatHeatersPresent()) {
        vehicleModeSignal_.subscribe([this]() { handleVehicleMode(); });
        if (userLocation == UserLocation::DRIVER) {
            sHeatLevel_.subscribe([this]() { request(sHeatLevel_.get()); });
        }
    } else {
        shareHeatAttribute_.set(FirstRowGen::HeatState::NOT_PRESENT, FirstRowGen::HeatLevel::OFF);
        sendSignal(toAutosar(FirstRowGen::HeatLevel::OFF));
    }
}

SeatHeatLogic::~SeatHeatLogic() { sHeatLevel_.unSubscribe(); }

autosar::SeatClimaLvl& SeatHeatLogic::getSeatHeatSignal() {
    auto const steeringWheelPos = carconfig::getValue<CarConfigParams::CC8_SteeringWheelPositionType>();

    log_debug() << "steeringWheelPos = " << static_cast<int>(steeringWheelPos);

    if (((steeringWheelPos == CarConfigParams::CC8_SteeringWheelPositionType::Left_Hand_Drive) &&
         (userLocation_ == UserLocation::DRIVER)) ||
        ((steeringWheelPos == CarConfigParams::CC8_SteeringWheelPositionType::Right_Hand_Drive) &&
         (userLocation_ == UserLocation::PASSENGER))) {
        return seatClimate_.HmiSeatHeatgForRowFirstLe;
    } else {
        return seatClimate_.HmiSeatHeatgForRowFirstRi;
    }
}

void SeatHeatLogic::handleSeatHeatAutoSignal() {
    if (activationCheck()) {
        if (seatHeatgAutCdnSignal_.get().isOk() && (seatHeatgAutCdnSignal_.get().value() == autosar::Flg1::Set)) {
            log_warn() << "seatHeatgAutCdnSignal_ is set";

            auto difference = timeProvider_.steady_clock_now() - initialTime_;
            if (difference < timeProviderTimeout_) {
                auto const level = autoSeatHeatLevel_.get().getCurrentLevel();
                log_debug() << "seatHeatgAutCdnSignal_ callback, autoSeatHeatOn_ is ON, set to auto, "
                            << toHeatLevel(level);
                shareHeatAttribute_.set(FirstRowGen::HeatState::AUTO, toHeatLevel(level));
                sendSignal(toAutosar(toHeatLevel(level)));
            } else {
                log_warn() << "Auto condition received after timeout; ignoring...";
            }
        }
    }
}

void SeatHeatLogic::handleAuto() {
    if ((autoSeatHeatOn_.get().getCurrentState() == UserSelectionGen::StateType::AVAILABLE) &&
        (autoSeatHeatOn_.get().getCurrentSelection() == UserSelectionGen::OffOnType::ON) && !inDynoMode()) {
        initialTime_ = timeProvider_.steady_clock_now();
        if (seatHeatgAutCdnSignal_.get().isOk() && seatHeatgAutCdnSignal_.get().value() == autosar::Flg1::Set) {
            auto const level = autoSeatHeatLevel_.get().getCurrentLevel();
            log_debug() << "HandleAuto, autoSeatHeatOn_ is ON, set to auto, " << toHeatLevel(level);
            shareHeatAttribute_.set(FirstRowGen::HeatState::AUTO, toHeatLevel(level));
            sendSignal(toAutosar(toHeatLevel(level)));
        } else {
            log_debug() << "HandleAuto, autoSeatHeatOn_ is ON, set to auto, OFF";
            shareHeatAttribute_.set(FirstRowGen::HeatState::AUTO, FirstRowGen::HeatLevel::OFF);
            sendSignal(toAutosar(FirstRowGen::HeatLevel::OFF));
        }
        seatHeatgAutCdnSignal_.subscribe([this] { handleSeatHeatAutoSignal(); });
    } else {
        log_debug() << "HandleAuto, autoSeatHeatOn_ is OFF, set to manual, " << sHeatLevel_.get();
        shareHeatAttribute_.set(FirstRowGen::HeatState::MANUAL, sHeatLevel_.get());
        seatHeatgAutCdnSignal_.subscribe([] {});
        sendSignal(toAutosar(sHeatLevel_.get()));
    }
}

void SeatHeatLogic::request(FirstRowGen::HeatLevel const level) {
    if (activationCheck() && seatHeatersPresent()) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        log_debug() << "SeatHeatLogic request, set to manual, " << level;
        shareHeatAttribute_.set(FirstRowGen::HeatState::MANUAL, level);
        sendSignal(toAutosar(level));
        sHeatLevel_.set(static_cast<FirstRowGen::HeatLevel::Literal>(level.value_));
    } else {
        log_warn() << "SeatHeatLogic request, when not active";
    }
}

bool SeatHeatLogic::activationCheck() const {
    auto carMode = vehicleModeSignal_.get().value().CarModSts1_;
    auto usageMode = vehicleModeSignal_.get().value().UsgModSts;

    auto carModeStatusOk = (carMode == autosar::CarModSts1::CarModNorm) || (carMode == autosar::CarModSts1::CarModDyno);
    auto usageModeOk = usageMode == autosar::UsgModSts1::UsgModDrvg;

    log_debug() << "SeatHeatLogic activationCheck returns: " << (carModeStatusOk && usageModeOk);

    return carModeStatusOk && usageModeOk;
}

bool SeatHeatLogic::signalsOk() const { return vehicleModeSignal_.get().isOk(); }

bool SeatHeatLogic::inDynoMode() const {
    return vehicleModeSignal_.get().value().CarModSts1_ == autosar::CarModSts1::CarModDyno;
}

void SeatHeatLogic::handleVehicleMode() {
    if (signalsOk()) {
        if (activationCheck()) {
            log_debug() << "SeatHeatLogic, "
                        << "Car Mode: " << static_cast<int>(vehicleModeSignal_.get().value().CarModSts1_)
                        << ", Usg Mode: " << static_cast<int>(vehicleModeSignal_.get().value().UsgModSts);
            handleAuto();
        } else {
            log_debug() << "SeatHeatLogic, set to DISABLED, off";
            shareHeatAttribute_.set(FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF);
            sendSignal(autosar::SeatClimaLvl::Off);
        }
    } else {
        log_error() << "SeatHeatLogic, signal in error state";
        shareHeatAttribute_.set(FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF);
        sendSignal(autosar::SeatClimaLvl::Off);
    }
}

void SeatHeatLogic::sendSignal(autosar::SeatClimaLvl level) {
    log_debug() << "Send SeatHeat signal = " << static_cast<int>(level);
    seatHeatSignal_ = level;
    seatClimateSignal_.send(seatClimate_);
}
