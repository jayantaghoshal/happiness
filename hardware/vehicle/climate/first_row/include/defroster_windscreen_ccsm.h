/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include "dispatcher.h"
#include "electric_defroster_windscreen_logic.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"

#include <chrono>
#include <Application_dataelement.h>
#include <mutex>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class DefrosterWindscreenCCSM
{
public:
    DefrosterWindscreenCCSM(
        ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&                maxDefroster,
        ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& windscreenDefroster,
        IDispatcher&                                                               dispatcher,
        IDefroster&                                                                electricDefrosterWindscreenLogic,
        IDefroster& electricDefrosterWindscreenPopupLogic,
        IDefroster& maxDefrosterLogic);

    ~DefrosterWindscreenCCSM() = default;

private:
    void updateButtonLed();

    void updateMaxDefroster();
    void setMaxDefrosterAndDefrosterWindscreen();

    ApplicationDataElement::DEReceiver<autosar::Btn4ForUsrSwtPanFrntReq_info>    defrosterButtonReq_;
    ApplicationDataElement::DESender<autosar::LiForBtn4ForUsrSwtPanFrntCmd_info> defrosterButtonLedReq_;

    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>&                maxDefroster_;
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& defrosterWindscreen_;

    IDispatcher&              dispatcher_;
    std::chrono::milliseconds dispatcherTimeout_;
    std::recursive_mutex      mutex_;

    std::vector<SubscriptionHandle> subscriptions_;

    IDefroster& electricDefrosterWindscreenLogic_;
    IDefroster& electricDefrosterWindscreenPopupLogic_;
    IDefroster& maxDefrosterLogic_;
};
