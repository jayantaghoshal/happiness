/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <Application_dataelement.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include "auto_climate_logic.h"
#include "climate_reset_logic.h"
#include "fan_level_front_logic.h"
#include "idefroster.h"
#include "notifiable_property.h"

#include <mutex>

class MaxDefrosterLogic : public IDefroster {
    using FirstRowGen = v0::org::volvocars::climate::FirstRow;

  public:
    MaxDefrosterLogic(NotifiableProperty<FirstRowGen::MaxDefrosterState>& state,
                      ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFrontState,
                      ReadOnlyNotifiableProperty<FirstRowGen::AirDistributionAngle>& airDistState,
                      ReadOnlyNotifiableProperty<FirstRowGen::AutoClimateState>& autoClimate,
                      ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset);
    ~MaxDefrosterLogic() = default;

    void request(FirstRowGen::MaxDefrosterRequest state);
    virtual void request(OnOff requestedState) override;

  private:
    void handleSignals(void);
    bool activationCheck(autosar::UsgModSts1 const usgModeSts1,
                         autosar::CarModSts1 const carModSts1,
                         autosar::OnOff1 const climaActive) const;
    bool signalsOk() const;

  private:
    NotifiableProperty<FirstRowGen::MaxDefrosterState>& shareMaxDefroster;
    ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& shareFanLevelFront;
    ReadOnlyNotifiableProperty<FirstRowGen::AirDistributionAngle>& shareAirDistribution;
    ReadOnlyNotifiableProperty<FirstRowGen::AutoClimateState>& shareAutoClimate;
    ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& shareClimateReset;

    autosar::CarModSts1 lastCarMode;
    autosar::UsgModSts1 lastUsgMode;
    autosar::OnOff1 lastClimaActive;

    // Subscription IDs
    SubscriptionHandle maxDefrosterId;
    SubscriptionHandle fanLevelFrontId;
    SubscriptionHandle airDistributionId;
    SubscriptionHandle autoClimateId;
    SubscriptionHandle climateResetId;

    bool isActive;

    // Autosar Signals
    ApplicationDataElement::DESender<autosar::HmiDefrstMaxReq_info> maxDefrosterSignal;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climaActiveSignal;

    std::recursive_mutex mutex;
};
