/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "infotainment_status.h"

//TODO(climateport): Commonprinters for infotainmentstatus
//#include "common_printers.h"
#include "logging_context.h"



namespace
{
const std::string sessionName = "TargetDefinitionState";
}

InfotainmentStatus::InfotainmentStatus(NotifiableProperty<State>& nodeSessionState)
    : nodeSessionState_{ nodeSessionState }
{
    //TODO(climateport): NSM
    //nodeState_.setNodeStateClient(this);
    //nodeSessionState_.set(static_cast<State>(nodeState_.GetSessionState(sessionName, NsmSeat_Driver)));
    //log_info() << LOG_PREFIX << "NSM initial nodeSessionMode: " << nodeSessionState_.get();
}

void InfotainmentStatus::onNodeState(const int)
{
}
void InfotainmentStatus::onNodeApplicationMode(const int)
{
}
void InfotainmentStatus::onSessionStateChanged(const std::string& sessionStateName,
                                               const int          seatId,
                                               const int          sessionState)
{
    //TODO(climateport): NSM
    /*
    if ((sessionName == sessionStateName) && (seatId == NsmSeat_Driver))
    {
        nodeSessionState_.set(static_cast<State>(sessionState));
        log_debug() << LOG_PREFIX << "NSM SessionState Changed: " << static_cast<State>(sessionState);
    }
    */
}
