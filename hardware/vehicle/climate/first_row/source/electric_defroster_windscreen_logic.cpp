/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "electric_defroster_windscreen_logic.h"

#include "autosar_printers.h"
#include "first_row_printers.h"
#include "local_config_helper.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace SettingsFramework;
using namespace autosar;
using namespace std::chrono_literals;

namespace
{
auto LOG_PREFIX = "ElectricDefrosterWindscreen: ";
}

ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenLogic(
    NotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& ElectricWindscreen,
    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>&      autoDefrosterFront,
    IDispatcher&                                                       timerDispatcher,
    autosar::HmiDefrstElecReq&                                         hmiDefrstElecReq)
    : electricDefrosterWindscreenState_{ ElectricWindscreen }
    , timeout_(std::chrono::milliseconds{
          static_cast<int>(util::readLocalConfig<double>("Climate_defroster_timeout") * 1000.0) })
    , autoFrontRequest_(autoDefrosterFront)
    , requestElectricDefrosterWindscreenState_(ElectricDefrosterWindscreenState::OFF)
    , usageModeLast_(autosar::UsgModSts1::UsgModAbdnd)
    , carModeLast_(autosar::CarModSts1::CarModDyno)
    , driveModeLast_(autosar::DrvModReqType1::Undefd)
    , hmiDefrosterWindscreenStatusLast_(autosar::ActrDefrstSts::Off)
    , timerDispatcher_(timerDispatcher)
    , hmiDefrstElecReq_(hmiDefrstElecReq)
{
    registerFsm();

    if (!carConfigOk())
    {
        transitionToState(InternalElectricDefrosterWindscreenState::CARCONFIG_INVALID);
    }
    else
    {
        transitionToState(InternalElectricDefrosterWindscreenState::SYSTEM_OK);
        vehicleModes_.subscribe([this]() { handleVehicleMode(); });
        driveMode_.subscribe([this]() { handleDriveMode(); });
        hmiDefrosterWindscreenStatus_.subscribe([this]() { handleHmiDefrosterStatus(); });

        handleDriveMode();
        handleHmiDefrosterStatus();
        handleVehicleMode();
    }
}

bool ElectricDefrosterWindscreenLogic::isInState(int state)
{
    return inState(state);
}

void ElectricDefrosterWindscreenLogic::request(ElectricDefrosterWindscreenRequest requestedState)
{
    log_debug() << LOG_PREFIX << "request = " << requestedState
                << "current = " << electricDefrosterWindscreenState_.get();
    if (electricDefrosterWindscreenState_.get() != requestedState)
    {
        if (inState(InternalElectricDefrosterWindscreenState::AUTO)
            || (inState(InternalElectricDefrosterWindscreenState::MANUAL)
                && hmiDefrosterWindscreenStatusLast_ != autosar::ActrDefrstSts::NotAvailable))
        {
            requestElectricDefrosterWindscreenState_.value_ = requestedState.value_;
            changeStateOnTrigger();
        }
    }
}

void ElectricDefrosterWindscreenLogic::request(OnOff requestedState)
{
    log_debug() << LOG_PREFIX << "requestCCSM " << requestedState;

    if (requestedState == OnOff::On)
    {
        request(FirstRowGen::ElectricDefrosterWindscreenRequest::ON);
    }
    else
    {
        request(FirstRowGen::ElectricDefrosterWindscreenRequest::OFF);
    }
}

bool ElectricDefrosterWindscreenLogic::carConfigOk()
{
    const CarConfigParams::CC122_HeatedWindscreenType HeatedWindscreen
        = carconfig::getValue<CarConfigParams::CC122_HeatedWindscreenType>();

    if (HeatedWindscreen == CarConfigParams::CC122_HeatedWindscreenType::Heated_Frontscreen)
    {
        return true;
    }

    return false;
}

void ElectricDefrosterWindscreenLogic::registerFsm()
{
    // Setup Main FSM
    createState(InternalElectricDefrosterWindscreenState::INIT);
    registerMainInitState(INIT);

    // Create all states
    createState(InternalElectricDefrosterWindscreenState::CARCONFIG_INVALID);
    createState(InternalElectricDefrosterWindscreenState::SYSTEM_OK);
    createState(InternalElectricDefrosterWindscreenState::SYSTEM_ERROR);
    createState(InternalElectricDefrosterWindscreenState::NOT_ACTIVE,
                InternalElectricDefrosterWindscreenState::SYSTEM_OK);
    createState(InternalElectricDefrosterWindscreenState::ACTIVE, InternalElectricDefrosterWindscreenState::SYSTEM_OK);
    createState(InternalElectricDefrosterWindscreenState::AUTO, InternalElectricDefrosterWindscreenState::ACTIVE);
    createState(InternalElectricDefrosterWindscreenState::AUTO_OFF, InternalElectricDefrosterWindscreenState::AUTO);
    createState(InternalElectricDefrosterWindscreenState::AUTO_ON, InternalElectricDefrosterWindscreenState::AUTO);
    createState(InternalElectricDefrosterWindscreenState::MANUAL, InternalElectricDefrosterWindscreenState::ACTIVE);
    createState(InternalElectricDefrosterWindscreenState::MANUAL_OFF, InternalElectricDefrosterWindscreenState::MANUAL);
    createState(InternalElectricDefrosterWindscreenState::MANUAL_ON, InternalElectricDefrosterWindscreenState::MANUAL);

    addInitState(InternalElectricDefrosterWindscreenState::SYSTEM_OK,
                 InternalElectricDefrosterWindscreenState::NOT_ACTIVE);
    addInitState(InternalElectricDefrosterWindscreenState::AUTO, InternalElectricDefrosterWindscreenState::AUTO_OFF);

    // Add entry, exit and body
    addStateEntry(InternalElectricDefrosterWindscreenState::NOT_ACTIVE, [this]() {
        log_debug() << LOG_PREFIX << "State: NOT_ACTIVE";
        setState(ElectricDefrosterWindscreenState::DISABLED);
        sendSignal(autosar::ActrReq::Off);
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::ACTIVE, [this]() {
        log_debug() << LOG_PREFIX << "State: ACTIVE";
        if (activeCheck())
        {
            transitionToState(InternalElectricDefrosterWindscreenState::AUTO);
        }
        else
        {
            transitionToState(InternalElectricDefrosterWindscreenState::MANUAL);
        }
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::AUTO, [this]() {
        log_debug() << LOG_PREFIX << "State: AUTO";
        executeTransition(InternalElectricDefrosterWindscreenState::AUTO,
                          InternalElectricDefrosterWindscreenState::AUTO_OFF);
        sendSignal(autosar::ActrReq::AutOn);
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::AUTO_ON, [this]() {
        log_debug() << LOG_PREFIX << "State: AUTO_ON";
        setState(ElectricDefrosterWindscreenState::ON);
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::AUTO_OFF, [this]() {
        log_debug() << LOG_PREFIX << "State: AUTO_OFF";
        setState(ElectricDefrosterWindscreenState::OFF);
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::MANUAL, [this]() {
        log_debug() << LOG_PREFIX << "State: MANUAL";
        if (initManualOnCheck())
        {
            executeTransition(InternalElectricDefrosterWindscreenState::MANUAL,
                              InternalElectricDefrosterWindscreenState::MANUAL_ON);
        }
        else
        {
            executeTransition(InternalElectricDefrosterWindscreenState::MANUAL,
                              InternalElectricDefrosterWindscreenState::MANUAL_OFF);
        }
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::MANUAL_ON, [this]() {
        log_debug() << LOG_PREFIX << "State: MANUAL_ON";
        log_verbose() << LOG_PREFIX << "Timer started = " << timeout_.count() << "ms";

        setState(ElectricDefrosterWindscreenState::ON);
        sendSignal(autosar::ActrReq::On);

        timerDispatcher_.Start(timeout_, [this]() {

            if ((hmiDefrosterWindscreenStatusLast_ != ActrDefrstSts::On
                 && hmiDefrosterWindscreenStatusLast_ != ActrDefrstSts::Limited))
            {
                transitionToState(InternalElectricDefrosterWindscreenState::MANUAL_OFF);
            }
        });
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::MANUAL_OFF, [this]() {
        log_debug() << LOG_PREFIX << "State: MANUAL_OFF";
        setState(ElectricDefrosterWindscreenState::OFF);
        sendSignal(autosar::ActrReq::Off);
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::SYSTEM_ERROR, [this]() {
        log_debug() << LOG_PREFIX << "State: SYSTEM_ERROR";
        setState(ElectricDefrosterWindscreenState::DISABLED);
        sendSignal(autosar::ActrReq::Off);
    });

    addStateEntry(InternalElectricDefrosterWindscreenState::CARCONFIG_INVALID, [this]() {
        log_debug() << LOG_PREFIX << "State: CAR_CONFIG";
        setState(ElectricDefrosterWindscreenState::NOT_PRESENT);
    });

    // Create transitions between states->
    createTransition(InternalElectricDefrosterWindscreenState::INIT,
                     InternalElectricDefrosterWindscreenState::CARCONFIG_INVALID);
    createTransition(InternalElectricDefrosterWindscreenState::INIT,
                     InternalElectricDefrosterWindscreenState::SYSTEM_OK);
    createTransition(InternalElectricDefrosterWindscreenState::SYSTEM_OK,
                     InternalElectricDefrosterWindscreenState::SYSTEM_ERROR,
                     [this]() { return !signalOK(); });
    createTransition(InternalElectricDefrosterWindscreenState::SYSTEM_ERROR,
                     InternalElectricDefrosterWindscreenState::SYSTEM_OK,
                     [this]() { return signalOK(); });
    createTransition(InternalElectricDefrosterWindscreenState::NOT_ACTIVE,
                     InternalElectricDefrosterWindscreenState::ACTIVE,
                     [this]() { return activationCheck(); });
    createTransition(InternalElectricDefrosterWindscreenState::ACTIVE,
                     InternalElectricDefrosterWindscreenState::NOT_ACTIVE,
                     [this]() { return !activationCheck(); });

    createTransition(InternalElectricDefrosterWindscreenState::ACTIVE, InternalElectricDefrosterWindscreenState::AUTO);
    createTransition(InternalElectricDefrosterWindscreenState::ACTIVE,
                     InternalElectricDefrosterWindscreenState::MANUAL);

    createTransition(InternalElectricDefrosterWindscreenState::AUTO,
                     InternalElectricDefrosterWindscreenState::AUTO_OFF);
    createTransition(InternalElectricDefrosterWindscreenState::AUTO,
                     InternalElectricDefrosterWindscreenState::MANUAL,
                     [this]() { return manualCheck(); });
    createTransition(InternalElectricDefrosterWindscreenState::AUTO_OFF,
                     InternalElectricDefrosterWindscreenState::AUTO_ON,
                     [this]() { return autoOnCheck(); });
    createTransition(InternalElectricDefrosterWindscreenState::AUTO_ON,
                     InternalElectricDefrosterWindscreenState::AUTO_OFF,
                     [this]() { return autoOffCheck(); });

    createTransition(InternalElectricDefrosterWindscreenState::MANUAL,
                     InternalElectricDefrosterWindscreenState::MANUAL_OFF);
    createTransition(InternalElectricDefrosterWindscreenState::MANUAL,
                     InternalElectricDefrosterWindscreenState::MANUAL_ON);
    createTransition(InternalElectricDefrosterWindscreenState::MANUAL_OFF,
                     InternalElectricDefrosterWindscreenState::MANUAL_ON,
                     [this]() { return manualOnCheck(); });
    createTransition(InternalElectricDefrosterWindscreenState::MANUAL_ON,
                     InternalElectricDefrosterWindscreenState::MANUAL_OFF,
                     [this]() { return manualOffCheck(); });
}

/*=====================================================*/
/*                  Subscribe Functions                */
/*=====================================================*/

// This signal is only considered when going from NOT_ACTIVE to ACTIVE
void ElectricDefrosterWindscreenLogic::handleDriveMode()
{
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    if (driveMode_.get().isOk())
    {
        driveModeLast_ = driveMode_.get().value();
        log_verbose() << LOG_PREFIX << "DriveMode : " << static_cast<int>(driveModeLast_);
    }

    if (checkSignalOK())
    {
        changeStateOnTrigger();
    }
}

void ElectricDefrosterWindscreenLogic::handleVehicleMode()
{
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    if (vehicleModes_.get().isOk())
    {
        carModeLast_   = vehicleModes_.get().value().CarModSts1_;
        usageModeLast_ = vehicleModes_.get().value().UsgModSts;
        log_verbose() << LOG_PREFIX << "CarMode_ : " << carModeLast_;
        log_verbose() << LOG_PREFIX << "UsageMode_ : " << usageModeLast_;
    }

    if (checkSignalOK())
    {
        changeStateOnTrigger();
    }
}

void ElectricDefrosterWindscreenLogic::handleHmiDefrosterStatus()
{
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    if (hmiDefrosterWindscreenStatus_.get().isOk())
    {
        if (hmiDefrosterWindscreenStatusLast_ != hmiDefrosterWindscreenStatus_.get().value().Frnt)
        {
            hmiDefrosterWindscreenStatusLast_ = hmiDefrosterWindscreenStatus_.get().value().Frnt;
            log_verbose() << LOG_PREFIX
                          << "HmiDefrosterWindscreenStatus_ : " << static_cast<int>(hmiDefrosterWindscreenStatusLast_);
        }
    }

    if (checkSignalOK())
    {
        changeStateOnTrigger();
    }
}

/*=====================================================*/
/*                  Check Functions                    */
/*=====================================================*/
bool ElectricDefrosterWindscreenLogic::activationCheck()
{
    return usageModeLast_ == autosar::UsgModSts1::UsgModDrvg
           && (carModeLast_ == autosar::CarModSts1::CarModDyno || carModeLast_ == autosar::CarModSts1::CarModNorm);
}

bool ElectricDefrosterWindscreenLogic::activeCheck()
{
    log_debug() << LOG_PREFIX
                << "Auto Front Value = " << static_cast<int>(autoFrontRequest_.get().getCurrentSelection());
    return autoFrontRequest_.get().getCurrentSelection() == UserSelectionGen::OffOnType::ON
           && (driveModeLast_ != autosar::DrvModReqType1::DrvMod1
               && driveModeLast_ != autosar::DrvModReqType1::DrvMod8);
}

bool ElectricDefrosterWindscreenLogic::autoOnCheck()
{
    return hmiDefrosterWindscreenStatusLast_ == autosar::ActrDefrstSts::AutoCdn;
}

bool ElectricDefrosterWindscreenLogic::autoOffCheck()
{
    return hmiDefrosterWindscreenStatusLast_ != autosar::ActrDefrstSts::AutoCdn;
}

bool ElectricDefrosterWindscreenLogic::manualCheck()
{
    return (hasChangedTo(ElectricDefrosterWindscreenState::ON)
            && electricDefrosterWindscreenState_.get() == ElectricDefrosterWindscreenState::OFF)
           || ((hasChangedTo(ElectricDefrosterWindscreenState::OFF)
                || hmiDefrosterWindscreenStatusLast_ == autosar::ActrDefrstSts::NotAvailable)
               && electricDefrosterWindscreenState_.get() == ElectricDefrosterWindscreenState::ON);
}

bool ElectricDefrosterWindscreenLogic::initManualOnCheck()
{
    return (hmiDefrosterWindscreenStatusLast_ != autosar::ActrDefrstSts::NotAvailable)
           && (electricDefrosterWindscreenState_.get() == ElectricDefrosterWindscreenState::OFF);
}

bool ElectricDefrosterWindscreenLogic::manualOffCheck()
{
    return (hasChangedTo(ElectricDefrosterWindscreenState::OFF)
            || hmiDefrosterWindscreenStatusLast_ == autosar::ActrDefrstSts::TmrOff
            || hmiDefrosterWindscreenStatusLast_ == autosar::ActrDefrstSts::NotAvailable
            || (!timerDispatcher_.IsRunning() && (hmiDefrosterWindscreenStatusLast_ != autosar::ActrDefrstSts::On)
                && (hmiDefrosterWindscreenStatusLast_ != autosar::ActrDefrstSts::Limited)));
}

bool ElectricDefrosterWindscreenLogic::manualOnCheck()
{
    return hmiDefrosterWindscreenStatusLast_ != autosar::ActrDefrstSts::NotAvailable
           && hasChangedTo(ElectricDefrosterWindscreenState::ON);
}
/*=====================================================*/
/*                  Help Functions                     */
/*=====================================================*/
void ElectricDefrosterWindscreenLogic::changeStateOnTrigger()
{
    if (inState(InternalElectricDefrosterWindscreenState::NOT_ACTIVE))
    {
        executeTransition(InternalElectricDefrosterWindscreenState::NOT_ACTIVE,
                          InternalElectricDefrosterWindscreenState::ACTIVE);
    }
    if (inState(InternalElectricDefrosterWindscreenState::ACTIVE))
    {
        executeTransition(InternalElectricDefrosterWindscreenState::ACTIVE,
                          InternalElectricDefrosterWindscreenState::NOT_ACTIVE);

        if (!inState(InternalElectricDefrosterWindscreenState::NOT_ACTIVE))
        {
            if (inState(InternalElectricDefrosterWindscreenState::AUTO))
            {
                executeTransition(InternalElectricDefrosterWindscreenState::AUTO,
                                  InternalElectricDefrosterWindscreenState::MANUAL);
            }
            if (inState(InternalElectricDefrosterWindscreenState::AUTO_OFF))
            {
                executeTransition(InternalElectricDefrosterWindscreenState::AUTO_OFF,
                                  InternalElectricDefrosterWindscreenState::AUTO_ON);
            }
            else if (inState(InternalElectricDefrosterWindscreenState::AUTO_ON))
            {
                executeTransition(InternalElectricDefrosterWindscreenState::AUTO_ON,
                                  InternalElectricDefrosterWindscreenState::AUTO_OFF);
            }
            if (inState(InternalElectricDefrosterWindscreenState::MANUAL_OFF))
            {
                executeTransition(InternalElectricDefrosterWindscreenState::MANUAL_OFF,
                                  InternalElectricDefrosterWindscreenState::MANUAL_ON);
            }
            else if (inState(InternalElectricDefrosterWindscreenState::MANUAL_ON))
            {
                executeTransition(InternalElectricDefrosterWindscreenState::MANUAL_ON,
                                  InternalElectricDefrosterWindscreenState::MANUAL_OFF);
            }
        }
    }
    if (inState(ElectricDefrosterWindscreenLogic::SYSTEM_ERROR))
    {
        executeTransition(InternalElectricDefrosterWindscreenState::SYSTEM_ERROR,
                          InternalElectricDefrosterWindscreenState::SYSTEM_OK);
    }
}

bool ElectricDefrosterWindscreenLogic::checkSignalOK()
{
    if (signalOK())
    {
        if (inState(InternalElectricDefrosterWindscreenState::SYSTEM_ERROR))
        {
            setState(localState_);
            transitionToState(InternalElectricDefrosterWindscreenState::SYSTEM_OK);
        }
        return true;
    }
    else
    {
        if (inState(InternalElectricDefrosterWindscreenState::SYSTEM_OK))
        {
            localState_ = electricDefrosterWindscreenState_.get();
            transitionToState(InternalElectricDefrosterWindscreenState::SYSTEM_ERROR);
        }
        return false;
    }
}

bool ElectricDefrosterWindscreenLogic::signalOK()
{
    auto ok = (driveMode_.get().isOk() && vehicleModes_.get().isOk() && hmiDefrosterWindscreenStatus_.get().isOk());

    if (!ok)
    {
        log_warning() << LOG_PREFIX << "Signals ok = " << driveMode_.get().isOk() << " : " << vehicleModes_.get().isOk()
                      << " : " << hmiDefrosterWindscreenStatus_.get().isOk();
    }

    return ok;
}

bool ElectricDefrosterWindscreenLogic::hasChangedTo(ElectricDefrosterWindscreenState value)
{
    return (electricDefrosterWindscreenState_.get() != value && requestElectricDefrosterWindscreenState_ == value);
}

void ElectricDefrosterWindscreenLogic::setState(ElectricDefrosterWindscreenState newState)
{
    electricDefrosterWindscreenState_.set(newState);
    requestElectricDefrosterWindscreenState_ = newState;
}

void ElectricDefrosterWindscreenLogic::sendSignal(autosar::ActrReq elecReq)
{
    hmiDefrstElecReq_.FrntElecReq = elecReq;
    hmiDefrosterRequest_.send(hmiDefrstElecReq_);
}
