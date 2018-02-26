/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "auto_climate_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

AutoClimateLogic::AutoClimateLogic(NotifiableProperty<FirstRowGen::AutoClimateState>&             autoClimate,
                                   NotifiableProperty<AutoClimateEvent>&                          autoClimateEvent,
                                   ReadOnlyNotifiableProperty<FirstRowGen::AirDistributionAngle>& airDistribution,
                                   ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>&   fanLevelFront,
                                   ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&    maxDefroster)
    : shareAutoClimate(autoClimate)
    , shareAutoClimateEvent(autoClimateEvent)
    , shareAirDistribution(airDistribution)
    , shareFanLevelFront(fanLevelFront)
    , shareMaxDefroster(maxDefroster)
    , parkingClimateActive_(false)
    , autoClimateRequested_(false)
    , carConfigOk_(false)
    , active_(false)
    , conditionsOk_(false)
{

    carConfigOk_ = carConfigState();

    if (carConfigOk_)
    {
        parkingClimate.subscribe([this]() { handleParkingClimate(); });
        vehicleModes.subscribe([this]() { handleVehicleMode(); });
        maxDefrosterId_    = shareMaxDefroster.subscribe([this](const auto&) { this->toggleHandler(); });
        fanLevelFrontId_   = shareFanLevelFront.subscribe([this](const auto&) { this->toggleHandler(); });
        airDistributionId_ = shareAirDistribution.subscribe([this](const auto&) { this->toggleHandler(); });
        shareAutoClimateEvent.set(AutoClimateLogic::AutoClimateEvent::NO_REQ);
        activationCheck();
    }
    else
    {
        log_warning() << "AutoClimateLogic: Invalid car config";
        setStateAndValue();
    }
}

AutoClimateLogic::~AutoClimateLogic()
{
}

void AutoClimateLogic::handleParkingClimate()
{
    std::lock_guard<Mutex> lock(mutex_);

    if (parkingClimate.get().isOk())
    {
        parkingClimateActive_ = (parkingClimate.get().value() == autosar::OnOff1::On) ? true : false;
        activationCheck();
    }
}

void AutoClimateLogic::handleVehicleMode()
{
    std::lock_guard<Mutex> lock(mutex_);

    if (vehicleModes.get().isOk())
    {
        usageMode_ = vehicleModes.get().value().UsgModSts;
        carMode_   = vehicleModes.get().value().CarModSts1_;
        activationCheck();
    }
}

// Handles stimuli from other climate models
void AutoClimateLogic::toggleHandler()
{
    std::lock_guard<Mutex> lock(mutex_);
    checkAutoClimateConditions();
}

bool AutoClimateLogic::carConfigState() const
{
    using HvacVariants  = CarConfigParams::CC175_HvacVariantsType;

    bool result = false;

    try
    {
        const HvacVariants hvacVariant = carconfig::getValue<HvacVariants>();

        result = hvacVariant == HvacVariants::Electronic_Climate_Control_2_zone
                 || hvacVariant == HvacVariants::Electronic_Climate_Control_4_zone
                 || hvacVariant == HvacVariants::HVAC_Small_1_zone || hvacVariant == HvacVariants::HVAC_Small_2_zone;
    }
    catch (const std::out_of_range& ia)
    {
        log_error() << "AutoClimateLogic::carConfigState: Car config was throwing an exeption: "
                           + std::string(ia.what());
    }
    return result;
}

void AutoClimateLogic::activationCheck()
{
    if ((usageMode_ == autosar::UsgModSts1::UsgModDrvg
         || (parkingClimateActive_ && usageMode_ != autosar::UsgModSts1::UsgModAbdnd))
        && (carMode_ == autosar::CarModSts1::CarModNorm || carMode_ == autosar::CarModSts1::CarModDyno))
    {
        if (!active_)
        {
            active_ = true;
            checkAutoClimateConditions();
        }
    }
    else if (active_)
    {
        active_ = false;
        setStateAndValue();
    }
}

void AutoClimateLogic::checkAutoClimateConditions()
{
    if (shareAirDistribution.get() == FirstRowGen::AirDistributionAngle::AUTO
        && (shareFanLevelFront.get() != FirstRowGen::FanLevelFrontValue::OFF
            && shareFanLevelFront.get() != FirstRowGen::FanLevelFrontValue::MAX)
        && shareMaxDefroster.get() != FirstRowGen::MaxDefrosterState::ON)
    {
        conditionsOk_ = true;
    }
    else
    {
        conditionsOk_ = false;
    }

    log_debug() << "AutoClimateLogic: Result of checking conditions: " << conditionsOk_
                << ", AirDistributionValue: " << shareAirDistribution.get()
                << ", FanLevelFrontValue: " << shareFanLevelFront.get()
                << ", MaxDefrosterValue: " << shareMaxDefroster.get();
    setStateAndValue();
}

void AutoClimateLogic::requestAutoClimate()
{
    autoClimateRequested_ = true;
    // Lets unsubscribe to functions which gives input, or they would make auto climate
    // check the auto climate conditions and turn off before all functions are called

    maxDefrosterId_.reset();
    fanLevelFrontId_.reset();
    airDistributionId_.reset();
    setStateAndValue();
    maxDefrosterId_    = shareMaxDefroster.subscribe([this](const auto&) { this->toggleHandler(); });
    fanLevelFrontId_   = shareFanLevelFront.subscribe([this](const auto&) { this->toggleHandler(); });
    airDistributionId_ = shareAirDistribution.subscribe([this](const auto&) { this->toggleHandler(); });
}

void AutoClimateLogic::setStateAndValue()
{
    AutoClimateState value = FirstRowGen::AutoClimateState::NOT_VISIBLE;

    if (!carConfigOk_)
    {
        value = FirstRowGen::AutoClimateState::NOT_VISIBLE;
    }
    else if (active_ && autoClimateRequested_)
    {
        value = FirstRowGen::AutoClimateState::ON;
        log_debug() << "AutoClimate, generating AutoClimateEvent = ACTIVATED";
        shareAutoClimateEvent.set(AutoClimateEvent::ACTIVATED);
        log_debug() << "AutoClimate, generating AutoClimateEvent = NO_REQ";
        shareAutoClimateEvent.set(AutoClimateEvent::NO_REQ);
        autoClimateRequested_ = false;
    }
    else if (active_ && conditionsOk_)
    {
        value = FirstRowGen::AutoClimateState::ON;
    }
    else if (active_ && !conditionsOk_)
    {
        value = FirstRowGen::AutoClimateState::OFF;
    }
    else if (!active_)
    {
        value = FirstRowGen::AutoClimateState::DISABLED;
    }

    log_debug() << "AutoClimateLogic, value: " << value;
    shareAutoClimate.set(value);
}
