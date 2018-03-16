/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "notifiable_property.h"

// TODO(climateport): NSM
//#include <NodeStateTypes.h>
#include "porting_helpers.h"

class InfotainmentStatus final {
  public:
    enum class State { SYS_OFF = 10, OFF = 11, ON = 12 };

    InfotainmentStatus(NotifiableProperty<State>& nodeSessionState);
    void onNodeState(int) NOLONGEROVERRIDE;
    void onNodeApplicationMode(int) NOLONGEROVERRIDE;
    void onSessionStateChanged(const std::string& sessionStateName, int seatId, int sessionState) NOLONGEROVERRIDE;

  private:
    // TODO(climateport): NSM
    // NotifiableProperty<State>& nodeSessionState_;
    // cedric::core::NodeState    nodeState_;
};
