/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "seat_vent_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <carconfig.h>
#include <functional>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using FirstRowGen = v0::org::volvocars::climate::FirstRow;


SeatVentLogic::SeatVentLogic(UserLocation                                    userLocation,
                             autosar::HmiSeatClima&                          seatClimate,
                             NotifiableProperty<FirstRowGen::VentAttribute>& ventAttribute,
                             SettingsProxyInterface<FirstRowGen::VentLevel::Literal>& seatVentLevelSetting)
    : userLocation_{ userLocation }
    , shareVentAttribute_{ ventAttribute }
    , sVentLevel_{ seatVentLevelSetting }
    , seatClimate_{seatClimate}
    , seatVentSignal_{getSeatVentSignal()}
{

    if (seatVentilationPresent())
    {
        vehicleModeSignal_.subscribe(std::bind(&SeatVentLogic::handleVehicleMode, this));
        if (userLocation == UserLocation::DRIVER)
        {
            sVentLevel_.subscribe([this]() { request(sVentLevel_.get()); });
        }
    }
    else
    {
        shareVentAttribute_.set(FirstRowGen::VentState::NOT_PRESENT, FirstRowGen::VentLevel::OFF);
    }
}

SeatVentLogic::~SeatVentLogic()
{
    sVentLevel_.unSubscribe();
}

autosar::SeatClimaLvl SeatVentLogic::convertVentLevelToAutosar(FirstRowGen::VentLevel level)
{
    switch (level)
    {
    case FirstRowGen::VentLevel::OFF:
        return autosar::SeatClimaLvl::Off;
    case FirstRowGen::VentLevel::LVL1:
        return autosar::SeatClimaLvl::Lvl1;
    case FirstRowGen::VentLevel::LVL2:
        return autosar::SeatClimaLvl::Lvl2;
    case FirstRowGen::VentLevel::LVL3:
        return autosar::SeatClimaLvl::Lvl3;
    default:
        return autosar::SeatClimaLvl::Off;
    }
}

bool SeatVentLogic::seatVentilationPresent()
{
    try
    {
        return carconfig::getValue<CarConfigParams::CC179_FrontSeatVentilationType>()
               == CarConfigParams::CC179_FrontSeatVentilationType::Ventilated_seats;
    }
    catch (std::out_of_range const& exception)
    {
        log_fatal() << "CarConfig threw an exception: " << exception.what();
        throw;
    }
}

autosar::SeatClimaLvl& SeatVentLogic::getSeatVentSignal()
{
    auto const steeringWheelPos
        = carconfig::getValue<CarConfigParams::CC8_SteeringWheelPositionType>();

    log_debug() << "steeringWheelPos = " << static_cast<int>(steeringWheelPos);

    if (((steeringWheelPos == CarConfigParams::CC8_SteeringWheelPositionType::Left_Hand_Drive)
         && (userLocation_ == UserLocation::DRIVER))
        || ((steeringWheelPos == CarConfigParams::CC8_SteeringWheelPositionType::Right_Hand_Drive)
            && (userLocation_ == UserLocation::PASSENGER)))
    {
        return seatClimate_.HmiSeatVentnForRowFirstLe;
    }
    else
    {
        return seatClimate_.HmiSeatVentnForRowFirstRi;
    }
}

void SeatVentLogic::request(FirstRowGen::VentLevel const level)
{
    if (activationCheckOk() && seatVentilationPresent())
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        log_debug() << "SeatVentLogic request, set to manual, " << level;
        shareVentAttribute_.set(FirstRowGen::VentState::MANUAL, level);
        sendSignal(convertVentLevelToAutosar(level));
        sVentLevel_.set(static_cast<FirstRowGen::VentLevel::Literal>(level.value_));
    }
    else
    {
        log_warn() << "SeatVentLogic request, when not active";
    }
}

bool SeatVentLogic::activationCheckOk() const
{
    auto carMode   = vehicleModeSignal_.get().value().CarModSts1_;
    auto usageMode = vehicleModeSignal_.get().value().UsgModSts;

    auto carModeStatusOk
        = ((carMode == autosar::CarModSts1::CarModNorm) || (carMode == autosar::CarModSts1::CarModDyno));
    auto usageModeOk = (usageMode == autosar::UsgModSts1::UsgModDrvg);

    log_debug() << "SeatVentLogic activationCheck returns: " << (carModeStatusOk && usageModeOk);

    return carModeStatusOk && usageModeOk;
}

void SeatVentLogic::handleVehicleMode()
{
    if (vehicleModeSignal_.get().isOk())
    {
        if (activationCheckOk())
        {
            log_debug() << "SeatVentLogic, "
                        << "Car Mode: " << static_cast<int>(vehicleModeSignal_.get().value().CarModSts1_)
                        << ", Usg Mode: " << static_cast<int>(vehicleModeSignal_.get().value().UsgModSts);

            log_debug() << "Setting Seat Ventilation, Manual Mode, " << sVentLevel_.get();
            shareVentAttribute_.set(FirstRowGen::VentState::MANUAL, sVentLevel_.get());
            sendSignal(convertVentLevelToAutosar(sVentLevel_.get()));
        }
        else
        {
            log_debug() << "SeatVentLogic, set to DISABLED, off";
            shareVentAttribute_.set(FirstRowGen::VentState::DISABLED, FirstRowGen::VentLevel::OFF);
            sendSignal(autosar::SeatClimaLvl::Off);
        }
    }
    else
    {
        log_error() << "SeatVentLogic, signal in error state";
        shareVentAttribute_.set(FirstRowGen::VentState::DISABLED, FirstRowGen::VentLevel::OFF);
        sendSignal(autosar::SeatClimaLvl::Off);
    }
}

void SeatVentLogic::sendSignal(autosar::SeatClimaLvl level)
{
    log_debug() << "Send SeatVentsignal = " << static_cast<int>(level);
    seatVentSignal_ = level;
    seatClimateSignal_.send(seatClimate_);
}
