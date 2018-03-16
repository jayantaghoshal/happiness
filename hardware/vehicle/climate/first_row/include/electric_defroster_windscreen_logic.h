/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <legacydispatcher.h>
#include <libsettings/setting.h>
#include <vcc/localconfig.h>
#include <mutex>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "dfsm.h"
#include "idefroster.h"
#include "notifiable_property.h"

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class ElectricDefrosterWindscreenLogic : private DFsm_Main, public IDefroster {
  public:
    using ElectricDefrosterWindscreenState = FirstRowGen::ElectricDefrosterWindscreenState;
    using ElectricDefrosterWindscreenRequest = FirstRowGen::ElectricDefrosterWindscreenRequest;

    enum InternalElectricDefrosterWindscreenState {
        INIT = 0,
        CARCONFIG_INVALID,
        SYSTEM_OK,
        NOT_ACTIVE,
        ACTIVE,
        AUTO,
        AUTO_OFF,
        AUTO_ON,
        MANUAL,
        MANUAL_OFF,
        MANUAL_ON,
        SYSTEM_ERROR
    };

    ElectricDefrosterWindscreenLogic(
            const vcc::LocalConfigReaderInterface* lcfg,
            NotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& ElectricWindscreen,
            ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoDefrosterFront,
            ILegacyDispatcher& timerDispatcher,
            autosar::HmiDefrstElecReq& hmiDefrstElecReq);

    // PA_REQUEST_Defrost_WindscreenState
    void request(ElectricDefrosterWindscreenRequest requestedState);
    virtual void request(OnOff requestedState) override;

    bool isInState(int state);

  private:
    bool carConfigOk();

    void registerFsm();

    // subscribe functions
    void handleDriveMode();
    void handleVehicleMode();
    void handleHmiDefrosterStatus();

    // Check functions
    bool activationCheck();
    bool activeCheck();
    bool autoOnCheck();
    bool autoOffCheck();
    bool initManualOnCheck();
    bool manualOnCheck();
    bool manualOffCheck();
    bool manualCheck();

    // Help functions
    bool hasChangedTo(ElectricDefrosterWindscreenState value);
    void changeStateOnTrigger();
    void changeState();
    bool checkSignalOK();
    bool signalOK();
    void setState(ElectricDefrosterWindscreenState newState);
    void sendSignal(autosar::ActrReq elecReq);

    // PA_ElectricDefrost_WindscreenState
    NotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& electricDefrosterWindscreenState_;

    std::chrono::milliseconds timeout_;
    std::recursive_mutex mutex_;

    // GET_S_Auto_Front_Defroster ?
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoFrontRequest_;

    // These values save last valid values when getting SYSTEM_ERROR...
    ElectricDefrosterWindscreenState localState_ = ElectricDefrosterWindscreenState::DISABLED;

    // Used by PA_REQUEST_Defrost_WindscreenState
    ElectricDefrosterWindscreenState requestElectricDefrosterWindscreenState_;

    // stored values
    autosar::UsgModSts1 usageModeLast_;
    autosar::CarModSts1 carModeLast_;
    autosar::DrvModReqType1 driveModeLast_;
    autosar::ActrDefrstSts hmiDefrosterWindscreenStatusLast_;
    autosar::ActrReq hmiElectricDefrosterWindscreenRequestLast_;

    ILegacyDispatcher& timerDispatcher_;
    autosar::HmiDefrstElecReq& hmiDefrstElecReq_;

    // FlexRay signals
    ECDDataElement::DESink<autosar::DrvModReq_info> driveMode_;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModes_;
    ApplicationDataElement::DEReceiver<autosar::HmiDefrstElecSts_info> hmiDefrosterWindscreenStatus_;
    ApplicationDataElement::DESender<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest_;
};
