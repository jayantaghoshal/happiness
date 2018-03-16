/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "electric_defroster_windscreen_logic.h"
#include "legacydispatcher.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <vcc/localconfig.h>
#include <chrono>
#include <mutex>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class DefrosterWindscreenCCSM {
  public:
    DefrosterWindscreenCCSM(
            const vcc::LocalConfigReaderInterface* lcfg,
            ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
            ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& defrosterWindscreen,
            ILegacyDispatcher& dispatcher,
            IDefroster& electricDefrosterWindscreenLogic,
            IDefroster& electricDefrosterWindscreenPopupLogic,
            IDefroster& maxDefrosterLogic);

    ~DefrosterWindscreenCCSM() = default;

  private:
    void updateButtonLed();

    void updateMaxDefroster();
    void setMaxDefrosterAndDefrosterWindscreen();

    ApplicationDataElement::DEReceiver<autosar::Btn4ForUsrSwtPanFrntReq_info> defrosterButtonReq_;
    ApplicationDataElement::DESender<autosar::LiForBtn4ForUsrSwtPanFrntCmd_info> defrosterButtonLedReq_;

    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster_;
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& defrosterWindscreen_;

    ILegacyDispatcher& dispatcher_;
    std::chrono::milliseconds dispatcherTimeout_;
    std::recursive_mutex mutex_;

    std::vector<SubscriptionHandle> subscriptions_;

    IDefroster& electricDefrosterWindscreenLogic_;
    IDefroster& electricDefrosterWindscreenPopupLogic_;
    IDefroster& maxDefrosterLogic_;
};
