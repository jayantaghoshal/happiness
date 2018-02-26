/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "fan_level_front_logic.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

#include <Application_dataelement.h>

class AutoClimateLogic final
{
    using FirstRowGen      = v0::org::volvocars::climate::FirstRow;
    using AutoClimateState = FirstRowGen::AutoClimateState;

public:
    enum class AutoClimateEvent
    {
        NO_REQ,
        ACTIVATED
    };

    explicit AutoClimateLogic(NotifiableProperty<FirstRowGen::AutoClimateState>&             autoClimate,
                              NotifiableProperty<AutoClimateEvent>&                          autoClimateEvent,
                              ReadOnlyNotifiableProperty<FirstRowGen::AirDistributionAngle>& airDistribution,
                              ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>&   fanLevelFront,
                              ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&    maxDefroster);

    ~AutoClimateLogic();

    void requestAutoClimate();
    void toggleHandler();

private:
    using Mutex = std::recursive_mutex;
    Mutex mutex_;

    NotifiableProperty<FirstRowGen::AutoClimateState>& shareAutoClimate;
    NotifiableProperty<AutoClimateEvent>&              shareAutoClimateEvent;

    SubscriptionHandle airDistributionId_;
    SubscriptionHandle fanLevelFrontId_;
    SubscriptionHandle maxDefrosterId_;

    autosar::UsgModSts1 usageMode_;
    autosar::CarModSts1 carMode_;

    ReadOnlyNotifiableProperty<FirstRowGen::AirDistributionAngle>& shareAirDistribution;
    ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>&   shareFanLevelFront;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&    shareMaxDefroster;

    // FlexRay signals
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info>          parkingClimate;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModes;

    bool parkingClimateActive_;
    bool autoClimateRequested_;
    bool carConfigOk_;
    bool active_;
    bool conditionsOk_;

    bool carConfigState() const;
    void                   activationCheck();
    void                   checkAutoClimateConditions();
    void                   setStateAndValue();

    void handleParkingClimate();
    void handleVehicleMode();

    std::string valueToString(AutoClimateState value);
};
