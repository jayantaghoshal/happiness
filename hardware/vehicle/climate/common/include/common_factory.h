/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "infotainment_status.h"

//TODO(climateport): NSM
//#include "nodestate_client.h"
#include "notifiable_property.h"
#include "temperature_converter.h"

//TODO(climateport): NSM
//#include <NodeStateTypes.h>
//#include <cedric/cedric_nodestate.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

class FirstRowFactory;

namespace second_and_third_row
{
class Factory;
}

namespace common
{
namespace daemon
{
using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class Factory final
{
    friend FirstRowFactory;
    friend second_and_third_row::Factory;

public:
    Factory();

    ReadOnlyNotifiableProperty<FirstRowGen::TemperatureSyncState>& getTemperatureSyncProperty();
    ReadOnlyNotifiableProperty<double>&                            getDriverTempProperty();
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&         getDriverTempHiLoNProperty();

    ReadOnlyNotifiableProperty<double>&                    getTempRearLeft();
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& getTempHiLoNRearLeft();
    ReadOnlyNotifiableProperty<double>&                    getTempRearRight();
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& getTempHiLoNRearRight();

    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& getMaxDefrosterProperty();

    // Node State Manager related functionality
    ReadOnlyNotifiableProperty<InfotainmentStatus::State>& getNodeSessionStateProperty();
    ReadOnlyNotifiableProperty<bool>&                      getWakeupByIPLMProperty();
    ReadOnlyNotifiableProperty<bool>&                      getShutdownInProgressProperty();
    NotifiableProperty<bool>&                              getDataSentByParkingClimateIPLinkProperty();

private:
    NotifiableProperty<FirstRowGen::TemperatureSyncState> temperatureSync_;
    NotifiableProperty<double>                            driverTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>         driverTempHiLoN_;
    NotifiableProperty<double>                            tempRearLeft_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>         tempHiLoNRearLeft_;
    NotifiableProperty<double>                            tempRearRight_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>         tempHiLoNRearRight_;
    NotifiableProperty<FirstRowGen::MaxDefrosterState>    maxDefroster_;

    TemperatureConverter temperatureConverter_;

    // Node State Manager related functionality
    //TODO(climateport): NSM removed
    //cedric::core::NodeState nodeState_;

    NotifiableProperty<InfotainmentStatus::State> nodeSessionState_;
    NotifiableProperty<bool>                      wakeupByIPLM_;
    NotifiableProperty<bool>                      shutdownInProgress_;

    InfotainmentStatus infotainmentStatus_;
    //TODO(climateport): NSM removed
    //NodeStateClient    nodeStateClient_;
};
}
}
