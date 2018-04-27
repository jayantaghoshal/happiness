/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "electric_defroster_rear_logic.h"

#include "first_row_printers.h"
#include "local_config_helper.h"
#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace SettingsFramework;
using namespace autosar;
using namespace std::chrono_literals;

namespace {
auto LOG_PREFIX = "ElectricDefrosterRear: ";
}

ElectricDefrosterRearLogic::ElectricDefrosterRearLogic(
        const vcc::LocalConfigReaderInterface* lcfg,
        NotifiableProperty<ElectricDefrosterRearState>& ElectricDefrosterRear,
        ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoDefrosterRear,
        ILegacyDispatcher& timerDispatcher,
        autosar::HmiDefrstElecReq& hmiDefrstElecReq)
    : electricDefrosterRearState_(ElectricDefrosterRear),
      timeout_(std::chrono::milliseconds{
              static_cast<int>(util::readLocalConfig<double>("Climate_defroster_timeout", lcfg) * 1000.0)}),
      autoRearRequest_(autoDefrosterRear),
      currentElectricDefrosterRearState_(ElectricDefrosterRearState::OFF),
      requestedElectricDefrosterRearState_(ElectricDefrosterRearState::OFF),
      usageModeLast_(autosar::UsgModSts1::UsgModAbdnd),
      carModeLast_(autosar::CarModSts1::CarModDyno),
      driveModeLast_(autosar::DrvModReqType1::Undefd),
      hmiDefrosterRearStatusLast_(autosar::ActrDefrstSts::Off),
      hmiDefrosterMirrorStatusLast_(autosar::ActrDefrstSts::Off),
      driveMode_(),
      timerDispatcher_(timerDispatcher),
      hmiDefrstElecReq_(hmiDefrstElecReq) {
    registerFsm();

    transitionToState(InternalElectricDefrosterRearState::SYSTEM_OK);

    vehicleMode_.subscribe([this]() { handleVehicleMode(); });
    driveMode_.subscribe([this]() { handleDriveMode(); });
    hmiDefrosterStatus_.subscribe([this]() { handleHmiDefrosterStatus(); });

    handleDriveMode();
    handleHmiDefrosterStatus();
    handleVehicleMode();
}

bool ElectricDefrosterRearLogic::isInState(int state) {
    return inState(state);
}

void ElectricDefrosterRearLogic::request(ElectricDefrosterRearRequest requestedState) {
    log_debug() << LOG_PREFIX << "request = " << requestedState << " current = " << electricDefrosterRearState_.get();
    if (electricDefrosterRearState_.get() != requestedState) {
        requestedElectricDefrosterRearState_.value_ = requestedState.value_;
        currentElectricDefrosterRearState_ = electricDefrosterRearState_.get();
        changeStateOnTrigger();
        currentElectricDefrosterRearState_ = electricDefrosterRearState_.get();
    }
}

void ElectricDefrosterRearLogic::request(OnOff requestedState) {
    log_debug() << LOG_PREFIX << "requestCCSM " << (requestedState == OnOff::On ? "ON" : "OFF");

    if (requestedState == OnOff::On) {
        request(FirstRowGen::ElectricDefrosterRearRequest::ON);
    } else {
        request(FirstRowGen::ElectricDefrosterRearRequest::OFF);
    }
}

void ElectricDefrosterRearLogic::registerFsm() {
    createState(InternalElectricDefrosterRearState::INIT);
    registerMainInitState(InternalElectricDefrosterRearState::INIT);

    createState(InternalElectricDefrosterRearState::SYSTEM_OK);
    createState(InternalElectricDefrosterRearState::SYSTEM_ERROR);
    createState(InternalElectricDefrosterRearState::NOT_ACTIVE, InternalElectricDefrosterRearState::SYSTEM_OK);
    createState(InternalElectricDefrosterRearState::ACTIVE, InternalElectricDefrosterRearState::SYSTEM_OK);
    createState(InternalElectricDefrosterRearState::AUTO, InternalElectricDefrosterRearState::ACTIVE);
    createState(InternalElectricDefrosterRearState::AUTO_ON, InternalElectricDefrosterRearState::AUTO);
    createState(InternalElectricDefrosterRearState::AUTO_OFF, InternalElectricDefrosterRearState::AUTO);
    createState(InternalElectricDefrosterRearState::MANUAL, InternalElectricDefrosterRearState::ACTIVE);
    createState(InternalElectricDefrosterRearState::MANUAL_ON, InternalElectricDefrosterRearState::MANUAL);
    createState(InternalElectricDefrosterRearState::MANUAL_OFF, InternalElectricDefrosterRearState::MANUAL);

    addInitState(InternalElectricDefrosterRearState::SYSTEM_OK, InternalElectricDefrosterRearState::NOT_ACTIVE);
    addInitState(InternalElectricDefrosterRearState::AUTO, InternalElectricDefrosterRearState::AUTO_OFF);

    addStateEntry(InternalElectricDefrosterRearState::NOT_ACTIVE, [this]() {
        log_debug() << LOG_PREFIX << "State: NOT_ACTIVE";
        setState(ElectricDefrosterRearState::DISABLED);
        sendSignal(autosar::ActrReq::Off);
    });

    addStateEntry(InternalElectricDefrosterRearState::ACTIVE, [this]() {
        log_debug() << LOG_PREFIX << "State: ACTIVE";
        if (activeCheck()) {
            executeTransition(InternalElectricDefrosterRearState::ACTIVE, InternalElectricDefrosterRearState::AUTO);
        } else {
            executeTransition(InternalElectricDefrosterRearState::ACTIVE, InternalElectricDefrosterRearState::MANUAL);
        }
    });

    addStateEntry(InternalElectricDefrosterRearState::AUTO, [this]() {
        log_debug() << LOG_PREFIX << "State: AUTO";
        sendSignal(autosar::ActrReq::On);
        executeTransition(InternalElectricDefrosterRearState::AUTO, InternalElectricDefrosterRearState::AUTO_OFF);
    });

    addStateEntry(InternalElectricDefrosterRearState::AUTO_ON, [this]() {
        log_debug() << LOG_PREFIX << "State: AUTO_ON";
        setState(ElectricDefrosterRearState::ON);
    });

    addStateEntry(InternalElectricDefrosterRearState::AUTO_OFF, [this]() {
        log_debug() << LOG_PREFIX << "State: AUTO_OFF";
        setState(ElectricDefrosterRearState::OFF);
        sendSignal(autosar::ActrReq::AutOn);
    });

    addStateEntry(InternalElectricDefrosterRearState::MANUAL, [this]() {
        log_debug() << LOG_PREFIX << "State: MANUAL";
        if (initManualOnCheck()) {
            executeTransition(InternalElectricDefrosterRearState::MANUAL,
                              InternalElectricDefrosterRearState::MANUAL_ON);
        } else {
            executeTransition(InternalElectricDefrosterRearState::MANUAL,
                              InternalElectricDefrosterRearState::MANUAL_OFF);
        }
    });

    addStateEntry(InternalElectricDefrosterRearState::MANUAL_ON, [this]() {
        log_debug() << LOG_PREFIX << "State: MANUAL_ON";
        log_debug() << LOG_PREFIX << "Timer Started = " << timeout_.count() << "ms";

        setState(ElectricDefrosterRearState::ON);
        sendSignal(autosar::ActrReq::On);

        timerDispatcher_.Start(timeout_, [this]() {
            if ((hmiDefrosterRearStatusLast_ != ActrDefrstSts::On &&
                 hmiDefrosterRearStatusLast_ != ActrDefrstSts::Limited) &&
                (hmiDefrosterMirrorStatusLast_ != ActrDefrstSts::On &&
                 hmiDefrosterMirrorStatusLast_ != ActrDefrstSts::Limited)) {
                transitionToState(InternalElectricDefrosterRearState::MANUAL_OFF);
            }
        });
    });

    addStateEntry(InternalElectricDefrosterRearState::MANUAL_OFF, [this]() {
        log_debug() << LOG_PREFIX << "State: MANUAL_OFF";
        setState(ElectricDefrosterRearState::OFF);
        sendSignal(autosar::ActrReq::Off);
    });

    addStateEntry(InternalElectricDefrosterRearState::SYSTEM_ERROR, [this]() {
        log_error() << LOG_PREFIX << "State: SYSTEM_ERROR";
        setState(ElectricDefrosterRearState::DISABLED);
        sendSignal(autosar::ActrReq::Off);
    });

    createTransition(InternalElectricDefrosterRearState::INIT, InternalElectricDefrosterRearState::SYSTEM_OK);
    createTransition(InternalElectricDefrosterRearState::SYSTEM_OK,
                     InternalElectricDefrosterRearState::SYSTEM_ERROR,
                     [this]() { return !signalOK(); });
    createTransition(InternalElectricDefrosterRearState::SYSTEM_ERROR,
                     InternalElectricDefrosterRearState::SYSTEM_OK,
                     [this]() { return signalOK(); });
    createTransition(InternalElectricDefrosterRearState::NOT_ACTIVE,
                     InternalElectricDefrosterRearState::ACTIVE,
                     [this]() { return activationCheck(); });
    createTransition(InternalElectricDefrosterRearState::ACTIVE,
                     InternalElectricDefrosterRearState::NOT_ACTIVE,
                     [this]() { return !activationCheck(); });

    createTransition(InternalElectricDefrosterRearState::ACTIVE, InternalElectricDefrosterRearState::AUTO);
    createTransition(InternalElectricDefrosterRearState::ACTIVE, InternalElectricDefrosterRearState::MANUAL);

    createTransition(InternalElectricDefrosterRearState::AUTO, InternalElectricDefrosterRearState::AUTO_OFF);
    createTransition(InternalElectricDefrosterRearState::AUTO, InternalElectricDefrosterRearState::MANUAL, [this]() {
        return manualCheck();
    });
    createTransition(InternalElectricDefrosterRearState::AUTO_OFF,
                     InternalElectricDefrosterRearState::AUTO_ON,
                     [this]() { return autoOnCheck(); });
    createTransition(InternalElectricDefrosterRearState::AUTO_ON,
                     InternalElectricDefrosterRearState::AUTO_OFF,
                     [this]() { return autoOffCheck(); });

    createTransition(InternalElectricDefrosterRearState::MANUAL, InternalElectricDefrosterRearState::MANUAL_OFF);
    createTransition(InternalElectricDefrosterRearState::MANUAL, InternalElectricDefrosterRearState::MANUAL_ON);
    createTransition(InternalElectricDefrosterRearState::MANUAL_ON,
                     InternalElectricDefrosterRearState::MANUAL_OFF,
                     [this]() { return manualOffCheck(); });
    createTransition(InternalElectricDefrosterRearState::MANUAL_OFF,
                     InternalElectricDefrosterRearState::MANUAL_ON,
                     [this]() { return manualOnCheck(); });
}

/*=====================================================*/
/*                  Subscribe Functions                */
/*=====================================================*/
void ElectricDefrosterRearLogic::handleHmiDefrosterStatus() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    autosar::HmiDefrstElecSts status = hmiDefrosterStatus_.get().value();

    if (hmiDefrosterStatus_.get().isOk()) {
        if (hmiDefrosterRearStatusLast_ != status.Re || hmiDefrosterMirrorStatusLast_ != status.Mirrr) {
            hmiDefrosterRearStatusLast_ = status.Re;
            hmiDefrosterMirrorStatusLast_ = status.Mirrr;
            log_verbose() << LOG_PREFIX
                          << "HmiDefrosterRearStatus   : " << static_cast<int>(hmiDefrosterRearStatusLast_);
            log_verbose() << LOG_PREFIX
                          << "HmiDefrosterMirrorStatus : " << static_cast<int>(hmiDefrosterMirrorStatusLast_);
        }
    }

    if (checkSignalOK()) {
        changeStateOnTrigger();
    }
}

void ElectricDefrosterRearLogic::handleDriveMode() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    if (driveMode_.get().isOk()) {
        driveModeLast_ = driveMode_.get().value();
        log_verbose() << LOG_PREFIX << "DriveMode : " << static_cast<int>(driveModeLast_);
    }

    if (checkSignalOK()) {
        changeStateOnTrigger();
    }
}

void ElectricDefrosterRearLogic::handleVehicleMode() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    if (vehicleMode_.get().isOk()) {
        usageModeLast_ = vehicleMode_.get().value().UsgModSts;
        carModeLast_ = vehicleMode_.get().value().CarModSts1_;
        log_verbose() << LOG_PREFIX << "CarMode   : " << static_cast<int>(carModeLast_);
        log_verbose() << LOG_PREFIX << "UsageMode : " << static_cast<int>(usageModeLast_);
    }

    if (checkSignalOK()) {
        changeStateOnTrigger();
    }
}

/*=====================================================*/
/*                  Check Functions                    */
/*=====================================================*/
bool ElectricDefrosterRearLogic::activationCheck() {
    return (usageModeLast_ == autosar::UsgModSts1::UsgModDrvg &&
            (carModeLast_ == autosar::CarModSts1::CarModDyno || carModeLast_ == autosar::CarModSts1::CarModNorm));
}
bool ElectricDefrosterRearLogic::activeCheck() {
    log_debug() << LOG_PREFIX << "Auto Rear Value = " << autoRearRequest_.get().getCurrentSelection();

    return autoRearRequest_.get().getCurrentSelection() == UserSelectionGen::OffOnType::ON &&
           (driveModeLast_ != autosar::DrvModReqType1::DrvMod1 && driveModeLast_ != autosar::DrvModReqType1::DrvMod8);
}

bool ElectricDefrosterRearLogic::autoOnCheck() {
    log_debug() << "ElectricDefrosterRear, hmiDefrosterRearStatusLast_ = "
                << static_cast<int>(hmiDefrosterRearStatusLast_);
    log_debug() << "ElectricDefrosterRear, hmiDefrosterMirrorStatusLast_ = "
                << static_cast<int>(hmiDefrosterMirrorStatusLast_);
    return (hmiDefrosterRearStatusLast_ == autosar::ActrDefrstSts::AutoCdn ||
            hmiDefrosterMirrorStatusLast_ == autosar::ActrDefrstSts::AutoCdn);
}

bool ElectricDefrosterRearLogic::autoOffCheck() {
    return (hmiDefrosterRearStatusLast_ != autosar::ActrDefrstSts::AutoCdn &&
            hmiDefrosterMirrorStatusLast_ != autosar::ActrDefrstSts::AutoCdn);
}
bool ElectricDefrosterRearLogic::manualCheck() {
    return ((hasChangedTo(ElectricDefrosterRearState::OFF) ||
             hmiDefrosterRearStatusLast_ == autosar::ActrDefrstSts::NotAvailable ||
             hmiDefrosterMirrorStatusLast_ == autosar::ActrDefrstSts::NotAvailable) ||
            (hasChangedTo(ElectricDefrosterRearState::ON)));
}

bool ElectricDefrosterRearLogic::initManualOnCheck() {
    return (hmiDefrosterRearStatusLast_ != autosar::ActrDefrstSts::NotAvailable &&
            hmiDefrosterMirrorStatusLast_ != autosar::ActrDefrstSts::NotAvailable &&
            electricDefrosterRearState_.get() == ElectricDefrosterRearState::OFF);
}

bool ElectricDefrosterRearLogic::manualOffCheck() {
    return (hmiDefrosterRearStatusLast_ == autosar::ActrDefrstSts::NotAvailable ||
            hmiDefrosterMirrorStatusLast_ == autosar::ActrDefrstSts::NotAvailable ||
            hasChangedTo(ElectricDefrosterRearState::OFF) ||
            hmiDefrosterRearStatusLast_ == autosar::ActrDefrstSts::TmrOff ||
            hmiDefrosterMirrorStatusLast_ == autosar::ActrDefrstSts::TmrOff ||
            (!timerDispatcher_.IsRunning() && hmiDefrosterRearStatusLast_ != autosar::ActrDefrstSts::On &&
             hmiDefrosterRearStatusLast_ != autosar::ActrDefrstSts::Limited &&
             hmiDefrosterMirrorStatusLast_ != autosar::ActrDefrstSts::On &&
             hmiDefrosterMirrorStatusLast_ != autosar::ActrDefrstSts::Limited));
}

bool ElectricDefrosterRearLogic::manualOnCheck() {
    return ((hmiDefrosterRearStatusLast_ != autosar::ActrDefrstSts::NotAvailable &&
             hmiDefrosterMirrorStatusLast_ != autosar::ActrDefrstSts::NotAvailable) &&
            (hasChangedTo(ElectricDefrosterRearState::ON)));
}

void ElectricDefrosterRearLogic::changeStateOnTrigger() {
    if (inState(InternalElectricDefrosterRearState::NOT_ACTIVE)) {
        executeTransition(InternalElectricDefrosterRearState::NOT_ACTIVE, InternalElectricDefrosterRearState::ACTIVE);
    }
    if (inState(InternalElectricDefrosterRearState::ACTIVE)) {
        executeTransition(InternalElectricDefrosterRearState::ACTIVE, InternalElectricDefrosterRearState::NOT_ACTIVE);
        if (!inState(InternalElectricDefrosterRearState::NOT_ACTIVE)) {
            if (inState(InternalElectricDefrosterRearState::AUTO)) {
                executeTransition(InternalElectricDefrosterRearState::AUTO, InternalElectricDefrosterRearState::MANUAL);
            }
            if (inState(InternalElectricDefrosterRearState::AUTO_OFF)) {
                executeTransition(InternalElectricDefrosterRearState::AUTO_OFF,
                                  InternalElectricDefrosterRearState::AUTO_ON);
            } else if (inState(InternalElectricDefrosterRearState::AUTO_ON)) {
                executeTransition(InternalElectricDefrosterRearState::AUTO_ON,
                                  InternalElectricDefrosterRearState::AUTO_OFF);
            }
            if (inState(InternalElectricDefrosterRearState::MANUAL_OFF)) {
                executeTransition(InternalElectricDefrosterRearState::MANUAL_OFF,
                                  InternalElectricDefrosterRearState::MANUAL_ON);
            } else if (inState(InternalElectricDefrosterRearState::MANUAL_ON)) {
                executeTransition(InternalElectricDefrosterRearState::MANUAL_ON,
                                  InternalElectricDefrosterRearState::MANUAL_OFF);
            }
        }
    } else if (inState(InternalElectricDefrosterRearState::SYSTEM_ERROR)) {
        log_debug() << LOG_PREFIX << "SYSTEM_ERROR => SYSTEM_OK";
        executeTransition(InternalElectricDefrosterRearState::SYSTEM_ERROR,
                          InternalElectricDefrosterRearState::SYSTEM_OK);
    }
}

bool ElectricDefrosterRearLogic::checkSignalOK() {
    if (signalOK()) {
        if (inState(InternalElectricDefrosterRearState::SYSTEM_ERROR)) {
            setState(localState_);
            executeTransition(InternalElectricDefrosterRearState::SYSTEM_ERROR,
                              InternalElectricDefrosterRearState::SYSTEM_OK);
        }
        return true;
    } else {
        if (inState(InternalElectricDefrosterRearState::SYSTEM_OK)) {
            localState_ = electricDefrosterRearState_.get();
            executeTransition(InternalElectricDefrosterRearState::SYSTEM_OK,
                              InternalElectricDefrosterRearState::SYSTEM_ERROR);
        }
        return false;
    }
}

bool ElectricDefrosterRearLogic::signalOK() {
    // TODO (ARTINFO-2827) Temporary remove drive mode since it is missing in the system at this moment.
    // Add drive mode onec it is available
    // auto ok = driveMode_.get().isOk() && vehicleMode_.get().isOk() && hmiDefrosterStatus_.get().isOk();
    auto ok = vehicleMode_.get().isOk() && hmiDefrosterStatus_.get().isOk();

    if (!ok) {
        log_warning() << LOG_PREFIX << "Signals ok = " << vehicleMode_.get().isOk() << " : "
                      << hmiDefrosterStatus_.get().isOk();
        // log_warning() << LOG_PREFIX << "Signals ok = " << driveMode_.get().isOk() << " : " <<
        // vehicleMode_.get().isOk()
        //              << " : " << hmiDefrosterStatus_.get().isOk();
    }

    return ok;
}

bool ElectricDefrosterRearLogic::hasChangedTo(ElectricDefrosterRearState value) {
    return ((currentElectricDefrosterRearState_ != value) && (requestedElectricDefrosterRearState_ == value));
}

void ElectricDefrosterRearLogic::setState(ElectricDefrosterRearState newState) {
    electricDefrosterRearState_.set(newState);
}

void ElectricDefrosterRearLogic::sendSignal(autosar::ActrReq elecReq) {
    hmiDefrstElecReq_.ReElecReq = elecReq;
    hmiDefrstElecReq_.MirrElecReq = elecReq;
    hmiElectricDefrosterRearRequest_.send(hmiDefrstElecReq_);
}
