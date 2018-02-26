/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "notifiable_property.h"

//TODO(climateport): NSM
//#include <NodeStateTypes.h>
#include "porting_helpers.h"

class InfotainmentStatus final 
{
public:
    enum class State
    {
        SYS_OFF = 10,
        OFF     = 11,
        ON      = 12
    };

    InfotainmentStatus(NotifiableProperty<State>& nodeSessionState);
    void onNodeState(int) NOLONGEROVERRIDE;
    void onNodeApplicationMode(int) NOLONGEROVERRIDE;
    void onSessionStateChanged(const std::string& sessionStateName, int seatId, int sessionState) NOLONGEROVERRIDE;

private:
    NotifiableProperty<State>& nodeSessionState_;
    
    
    //TODO(climateport): NSM 
    //cedric::core::NodeState    nodeState_;
};
