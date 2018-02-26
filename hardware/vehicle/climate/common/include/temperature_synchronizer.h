/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "itemperature_logic.h"
#include "notifiable_property.h"

#include <gen_datatypes.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

namespace common
{
namespace daemon
{

class TemperatureSynchronizer
{
public:
    TemperatureSynchronizer(common::daemon::ITemperatureLogic&                             passengerTemperatureLogic,
                            ReadOnlyNotifiableProperty<FirstRowGen::TemperatureSyncState>& temperatureSync,
                            ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&    maxDefroster,
                            ReadOnlyNotifiableProperty<double>&                            driverTemp,
                            ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&         driverTempHiLoN);

    virtual ~TemperatureSynchronizer() = default;

protected:
    void synchronize(double temp, autosar::HmiCmptmtTSpSpcl tempHiLoN);

private:
    common::daemon::ITemperatureLogic& passengerTemperatureLogic_;

    ReadOnlyNotifiableProperty<FirstRowGen::TemperatureSyncState>& temperatureSync_;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&    maxDefroster_;

protected:
    ReadOnlyNotifiableProperty<double>&                    driverTemp_;
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& driverTempHiLoN_;
    std::vector<SubscriptionHandle>                        subscriptions_;
};
}
}
