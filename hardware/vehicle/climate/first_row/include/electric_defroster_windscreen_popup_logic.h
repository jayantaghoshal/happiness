/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include "electric_defroster_windscreen_logic.h"
#include "idefroster.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <mutex>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class ElectricDefrosterWindscreenPopupLogic : public IDefroster
{
public:
    using ElectricDefrosterWindscreenPopupState = FirstRowGen::ElectricDefrosterPopupState;

    explicit ElectricDefrosterWindscreenPopupLogic(
        NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>&              electricWindscreenPopupAttribute,
        ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& ElectricDefrosterWindscreen);

    ~ElectricDefrosterWindscreenPopupLogic();

    // PA_REQUEST_Defrost_WindscreenPopupState
    void requestElectricDefrosterWindscreenPopup(FirstRowGen::ElectricDefrosterPopupRequest requestedState);
    void requestElectricDefrosterWindscreen(FirstRowGen::ElectricDefrosterWindscreenRequest requestState);
    virtual void request(OnOff requestedState) override;

private:
    std::recursive_mutex                          mutex_;
    FirstRowGen::ElectricDefrosterWindscreenState previousWindowscreenState_;

    autosar::ActrDefrstSts HmiDefrosterWindscreenStatus_;

    NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>&              shareWindscreenPopupAttribute_;
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& shareElectricDefrosterWindscreen;
    SubscriptionHandle                                                         ElectricDefrosterWindscreenHandle;

    // FlexRay signals
    ApplicationDataElement::DEReceiver<autosar::HmiDefrstElecSts_info> HmiDefrosterStatus;

    // subscribe functions
    void handleHmiDefrosterStatus();
    void handleccsmDefrosterStatus();
    void handleElectricDefrosterWindscreen();
};
