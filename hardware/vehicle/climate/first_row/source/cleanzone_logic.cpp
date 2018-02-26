/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "cleanzone_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

bool CleanZoneLogic::carConfigState()
{
    bool result{ false };

    try
    {
        const CC174_AirQualitySystemType airQualitySystem = carconfig::getValue<CC174_AirQualitySystemType>();

        if (airQualitySystem == CC174_AirQualitySystemType::Air_Quality_System)
        {
            result = true;
        }
    }
    catch (const std::exception& ia)
    {
        log_error() << "CleanZone:carConfigState: Car config was throwing an exeption: " + std::string(ia.what());
    }
    return result;
}

CleanZoneLogic::CleanZoneLogic(NotifiableProperty<FirstRowGen::CleanzoneState>& cleanZoneIndication)
    : FsmState_(InternalState::INIT)
    , cleanZoneIndication_(cleanZoneIndication)
{
    log_info() << "CleanZoneLogic::CleanZoneLogic";

    if (carConfigState() == true)
    {
        setFsmState(NOT_ACTIVE);
        vehicleModes.subscribe([this]() { handleUsageAndCarMode(); });
        levelOfClimateComfort.subscribe([this]() { handleClimateComfort(); });
        climateActive.subscribe([this]() { handleClimateActive(); });
    }
    else
    {
        setFsmState(CARCONFIG_INVALID);
    }
    evaluateAndSetRightState();
}

CleanZoneLogic::~CleanZoneLogic()
{
    log_info() << "CleanZoneLogic::~CleanZoneLogic";
}

CleanZoneLogic::InternalState CleanZoneLogic::setFsmState(CleanZoneLogic::InternalState newState)
{
    switch (FsmState_)
    {
    case INIT:
        if (newState == CARCONFIG_INVALID || newState == NOT_ACTIVE)
        {
            FsmState_ = newState;
        }
        break;
    case NOT_ACTIVE:
        if (newState == ACTIVE || newState == SYSTEM_ERROR)
        {
            FsmState_ = newState;
        }
        break;
    case ACTIVE:
        if (newState == NOT_ACTIVE || newState == SYSTEM_ERROR)
        {
            FsmState_ = newState;
        }
        break;
    case SYSTEM_ERROR:
        if (newState == NOT_ACTIVE || newState == ACTIVE)
        {
            FsmState_ = newState;
        }
        break;
    case CARCONFIG_INVALID:
        // Do Nothing
        break;
    }

    if (newState != FsmState_)
        log_error() << "CleanZone: Could not switch from state " << FsmState_ << " to state " << newState;

    log_info() << "State changed to : " << FsmState_;
    return FsmState_;
}

void CleanZoneLogic::handleClimateComfort()
{
    std::lock_guard<std::mutex> locker(mutex_);

    levelOfClimateComfort_ = levelOfClimateComfort.get().value();

    handleSignalStatus();
    evaluateAndSetRightState();
}

void CleanZoneLogic::handleClimateActive()
{
    std::lock_guard<std::mutex> locker(mutex_);

    ClimateActive_ = climateActive.get().value();

    handleSignalStatus();
    evaluateAndSetRightState();
}

void CleanZoneLogic::handleSignalStatus()
{
    bool signalError
        = (vehicleModes.get().isError() || climateActive.get().isError() || levelOfClimateComfort.get().isError());

    switch (FsmState_)
    {
    case INIT:
    case CARCONFIG_INVALID:
        // Do Nothing
        break;
    case SYSTEM_ERROR:
        if (!signalError)
        {
            if (activationCheck())
                setFsmState(ACTIVE);
            else
                setFsmState(NOT_ACTIVE);
        }
        break;
    case NOT_ACTIVE:
        if (signalError)
        {
            setFsmState(SYSTEM_ERROR);
        }
        else
        {
            if (activationCheck())
            {
                setFsmState(ACTIVE);
            }
            else
            {
                setFsmState(NOT_ACTIVE);
            }
        }
        break;
    case ACTIVE:
        if (signalError)
        {
            setFsmState(SYSTEM_ERROR);
        }
        else
        {
            if (activationCheck())
            {
                setFsmState(ACTIVE);
            }
            else
            {
                setFsmState(NOT_ACTIVE);
            }
        }
        break;
    }
}

void CleanZoneLogic::handleUsageAndCarMode()
{
    std::lock_guard<std::mutex> locker(mutex_);

    UsageMode_ = vehicleModes.get().value().UsgModSts;
    CarMode_   = vehicleModes.get().value().CarModSts1_;

    handleSignalStatus();
    evaluateAndSetRightState();
}

void CleanZoneLogic::evaluateAndSetRightState()
{
    switch (FsmState_)
    {
    case INIT:
        setState(FirstRowGen::CleanzoneState::OFF);
        break;
    case NOT_ACTIVE:
        setState(FirstRowGen::CleanzoneState::NOT_OK);
        break;
    case ACTIVE:
        if (levelOfClimateComfort_ == autosar::LvlOfClimaCmft::Lvl2)
        {
            setState(FirstRowGen::CleanzoneState::OK);
        }
        else
        {
            setState(FirstRowGen::CleanzoneState::NOT_OK);
        }
        break;
    case SYSTEM_ERROR:
        setState(FirstRowGen::CleanzoneState::NOT_OK);
        break;
    case CARCONFIG_INVALID:
        setState(FirstRowGen::CleanzoneState::OFF);
        break;
    }
}

bool CleanZoneLogic::activationCheck()
{
    return ((UsageMode_ == UsgModSts1::UsgModDrvg
             || (ClimateActive_ == OnOff1::On && UsageMode_ != UsgModSts1::UsgModAbdnd))
            && (CarMode_ == CarModSts1::CarModNorm || CarMode_ == CarModSts1::CarModDyno));
}

void CleanZoneLogic::setState(FirstRowGen::CleanzoneState state)
{
    log_debug() << "setState: " << state;
    cleanZoneIndication_.set(state);
}
