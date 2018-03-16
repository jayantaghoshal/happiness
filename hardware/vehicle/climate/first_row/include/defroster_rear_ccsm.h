/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "electric_defroster_rear_logic.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class DefrosterRearCCSM {
  public:
    explicit DefrosterRearCCSM(ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& rearDefroster,
                               IDefroster& electricDefrosterRearLogic,
                               IDefroster& electricDefrosterRearPopupLogic);

    ~DefrosterRearCCSM() = default;

  private:
    void updateButtonLed();
    void updateRearDefroster();

    ApplicationDataElement::DEReceiver<autosar::Btn5ForUsrSwtPanFrntReq_info> defrosterRearButtonReq_;
    ApplicationDataElement::DESender<autosar::LiForBtn5ForUsrSwtPanFrntCmd_info> rearDefrosterButtonLedReq_;

    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& rearDefroster_;

    std::vector<SubscriptionHandle> subscriptions_;

    IDefroster& electricDefrosterRearLogic_;
    IDefroster& electricDefrosterRearPopupLogic_;
};
