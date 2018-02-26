/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include "idefroster.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using namespace autosar;

class ElectricDefrosterRearPopupLogic final : public IDefroster
{
public:
    ElectricDefrosterRearPopupLogic(
        ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& sElectricDefrosterRear,
        NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>&        sElectricDefrosterPopup);

    ~ElectricDefrosterRearPopupLogic();

    void request(FirstRowGen::ElectricDefrosterPopupRequest requestState);
    void requestElectricDefrosterRear(FirstRowGen::ElectricDefrosterRearRequest requestState);
    virtual void request(OnOff requestedState) override;

private:
    enum class InternalPopupState
    {
        OFF,
        ON
    };

    void setState(FirstRowGen::ElectricDefrosterPopupState state);
    void showPopup();
    void handleHmiDefrosterStatus();
    void handleElectricDefrosterRear();

    // PA_ElectricDefrost_RearState
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& shareElectricDefrosterRear;

    // PA_Popup_Electric_Defrost_RearState
    NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>& shareElectricDefrosterRearPopup;

    std::recursive_mutex                     mutex_;
    InternalPopupState                       CurrentState_;
    FirstRowGen::ElectricDefrosterRearState  previousRearState_;
    FirstRowGen::ElectricDefrosterPopupState requestedState_;

    ActrDefrstSts      HmiDefrosterRearStatus_;
    ActrDefrstSts      HmiDefrosterMirrorStatus_;
    SubscriptionHandle ElectricDefrosterRearHandle;

    // FlexRay signals
    ApplicationDataElement::DEReceiver<HmiDefrstElecSts_info> HmiDefrosterStatus;
};
