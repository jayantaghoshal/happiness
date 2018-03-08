/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "dfsm.h"
#include "idefroster.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <legacydispatcher.h>
#include <libsettings/setting.h>
#include <vcc/localconfig.h>
#include <mutex>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <v0/org/volvocars/climate/UserSelection.hpp>

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class ElectricDefrosterRearLogic final : private DFsm_Main, public IDefroster {
  public:
    using ElectricDefrosterRearState = FirstRowGen::ElectricDefrosterRearState;
    using ElectricDefrosterRearRequest = FirstRowGen::ElectricDefrosterRearRequest;

    enum InternalElectricDefrosterRearState {
        INIT = 0,
        SYSTEM_OK,
        NOT_ACTIVE,
        ACTIVE,
        AUTO,
        AUTO_ON,
        AUTO_OFF,
        MANUAL,
        MANUAL_ON,
        MANUAL_OFF,
        SYSTEM_ERROR
    };

    ElectricDefrosterRearLogic(const vcc::LocalConfigReaderInterface* lcfg,
                               NotifiableProperty<ElectricDefrosterRearState>& ElectricDefrosterRear,
                               ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoDefrosterRear,
                               ILegacyDispatcher& timerDispatcher, autosar::HmiDefrstElecReq& hmiDefrstElecReq);

    ~ElectricDefrosterRearLogic() = default;

    // PA_REQUEST_Defrost_RearState
    void request(ElectricDefrosterRearRequest requestedState);
    void request(OnOff requestedState) override;

    bool isInState(int state);

  private:
    // LIN button 5
    void registerFsm();

    // subscribe functions
    void handleHmiDefrosterStatus();
    void handleDriveMode();
    void handleVehicleMode();

    // Check functions
    bool activationCheck();
    bool activeCheck();
    bool manualCheck();
    bool autoOffCheck();
    bool autoOnCheck();
    bool initManualOnCheck();
    bool manualOffCheck();
    bool manualOnCheck();

    // help functions
    void changeStateOnTrigger();
    bool checkSignalOK();
    bool signalOK();
    bool hasChangedTo(ElectricDefrosterRearState value);
    void setState(ElectricDefrosterRearState newState);
    void sendSignal(autosar::ActrReq elecReq);

    // PA_ElectricDefrost_RearState
    NotifiableProperty<ElectricDefrosterRearState>& electricDefrosterRearState_;

    std::chrono::milliseconds timeout_;
    std::recursive_mutex mutex_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoRearRequest_;

    // These values save last valid values when getting SYSTEM_ERROR...
    ElectricDefrosterRearState localState_ = ElectricDefrosterRearState::DISABLED;

    // Used by PA_REQUEST_Defrost_WindscreenState
    ElectricDefrosterRearState currentElectricDefrosterRearState_;
    ElectricDefrosterRearState requestedElectricDefrosterRearState_;

    // Last values used for flank triggering
    autosar::UsgModSts1 usageModeLast_;
    autosar::CarModSts1 carModeLast_;
    autosar::DrvModReqType1 driveModeLast_;
    autosar::ActrDefrstSts hmiDefrosterRearStatusLast_;
    autosar::ActrDefrstSts hmiDefrosterMirrorStatusLast_;

    // FlexRay signals
    ECDDataElement::DESink<autosar::DrvModReq_info> driveMode_;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleMode_;

    // Both for mirror and Rear
    ApplicationDataElement::DESender<autosar::HmiDefrstElecReq_info> hmiElectricDefrosterRearRequest_;
    ApplicationDataElement::DEReceiver<autosar::HmiDefrstElecSts_info> hmiDefrosterStatus_;

    ILegacyDispatcher& timerDispatcher_;
    autosar::HmiDefrstElecReq& hmiDefrstElecReq_;
};
