/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#ifndef CLEANZONELOGIC_H
#define CLEANZONELOGIC_H

#include "notifiable_property.h"
#include <Application_dataelement.h>
#include <mutex>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class CleanZoneLogic
{
public:
    enum CleanZoneIndicationState
    {
        OFF,
        NOT_OK,
        OK
    };

    explicit CleanZoneLogic(NotifiableProperty<FirstRowGen::CleanzoneState>& cleanZoneIndication);
    ~CleanZoneLogic();

private:
    enum InternalState
    {
        INIT = 0,
        CARCONFIG_INVALID,
        NOT_ACTIVE,
        ACTIVE,
        SYSTEM_ERROR
    };

    bool carConfigState();

    // Finite State Machine functions
    InternalState setFsmState(InternalState newState);
    void setFsmEntry();
    void setState(FirstRowGen::CleanzoneState state);

    InternalState FsmState_;

    void handleClimateComfort();
    void handleUsageAndCarMode();
    void handleClimateActive();
    void handleSignalStatus();
    void evaluateAndSetRightState();
    bool activationCheck();

    NotifiableProperty<FirstRowGen::CleanzoneState>& cleanZoneIndication_;
    autosar::UsgModSts1                              UsageMode_;
    autosar::CarModSts1                              CarMode_;
    autosar::LvlOfClimaCmft                          levelOfClimateComfort_;
    autosar::OnOff1                                  ClimateActive_;

    unsigned int returnId_;
    std::mutex   mutex_;

    // FlexRay signal
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModes;
    ApplicationDataElement::DEReceiver<autosar::LvlOfClimaCmft_info>     levelOfClimateComfort;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info>          climateActive;
};

#endif // CLEANZONELOGIC_H
