/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "common_factory.h"

namespace
{
const std::uint32_t shutdownDelay_ms = 2000;
}

namespace common
{

namespace daemon
{

Factory::Factory()
    : temperatureSync_{ FirstRowGen::TemperatureSyncState::NOT_PRESENT }
    , driverTemp_{ 22.0 }
    , driverTempHiLoN_{ autosar::HmiCmptmtTSpSpcl::Norm }
    , tempRearLeft_{ 22 }
    , tempHiLoNRearLeft_{ autosar::HmiCmptmtTSpSpcl::Norm }
    , tempRearRight_{ 22 }
    , tempHiLoNRearRight_{ autosar::HmiCmptmtTSpSpcl::Norm }
    , maxDefroster_{ FirstRowGen::MaxDefrosterState::OFF }
    , temperatureConverter_{}
    //TODO(climateport): NSM
    //, nodeState_{ NSM_SHUTDOWNTYPE_NORMAL, shutdownDelay_ms }
    , nodeSessionState_{ InfotainmentStatus::State::SYS_OFF }
    , wakeupByIPLM_{ false }
    , shutdownInProgress_{ false }
    , infotainmentStatus_{ nodeSessionState_ }
    //TODO(climateport): NSM
    //, nodeStateClient_{ nodeState_, wakeupByIPLM_, shutdownInProgress_ }
{
}

ReadOnlyNotifiableProperty<FirstRowGen::TemperatureSyncState>& Factory::getTemperatureSyncProperty()
{
    return temperatureSync_;
}

ReadOnlyNotifiableProperty<double>& Factory::getDriverTempProperty()
{
    return driverTemp_;
}

ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& Factory::getDriverTempHiLoNProperty()
{
    return driverTempHiLoN_;
}

ReadOnlyNotifiableProperty<double>& Factory::getTempRearLeft()
{
    return tempRearLeft_;
}

ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& Factory::getTempHiLoNRearLeft()
{
    return tempHiLoNRearLeft_;
}

ReadOnlyNotifiableProperty<double>& Factory::getTempRearRight()
{
    return tempRearRight_;
}

ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& Factory::getTempHiLoNRearRight()
{
    return tempHiLoNRearRight_;
}

ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& Factory::getMaxDefrosterProperty()
{
    return maxDefroster_;
}

ReadOnlyNotifiableProperty<bool>& Factory::getWakeupByIPLMProperty()
{
    return wakeupByIPLM_;
}

ReadOnlyNotifiableProperty<bool>& Factory::getShutdownInProgressProperty()
{
    return shutdownInProgress_;
}

}
}
