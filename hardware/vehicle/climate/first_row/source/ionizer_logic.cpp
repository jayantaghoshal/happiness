/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "ionizer_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

namespace first_row
{
namespace
{

auto const LOG_PREFIX = "IonizerLogic: ";

bool isPresent()
{
    try
    {
        using CC349 = CarConfigParams::CC349_IonicInternalAirCleanerType;

        auto const param = carconfig::getValue<CC349>();

        return param == CC349::With_Ionic_internal_air_cleaner;
    }
    catch (std::out_of_range const& e)
    {
        log_fatal() << LOG_PREFIX << "CarConfig threw: " << e.what();
        throw;
    }
}
}

IonizerLogic::IonizerLogic(NotifiableProperty<FirstRowGen::IonizerState>& ionizer, SettingsProxy<bool, SettingsFramework::UserScope::NOT_USER_RELATED, SettingsFramework::UserScope::NOT_USER_RELATED>& ionizerSetting)
    : ionizer_{ ionizer }
    , ionizerSetting_{ ionizerSetting }
    , vehicleModeSignal_{}
    , climaActvSignal_{}
    , airClngReqSignal_{}
    , carConfigOk_{ false }
    , isActive_{ false }
    , isOk_{ false }
    , mutex_{}
{
    carConfigOk_ = isPresent();

    if (carConfigOk_)
    {
        ionizerSetting_.subscribe([this] {
            std::lock_guard<Mutex> lock(mutex_);
            log_debug() << LOG_PREFIX << "sIonizer changed";
            update();
        });

        auto const handleSignals = [this] {
            log_debug() << LOG_PREFIX << "handleSignals called";
            activationCheck();
            update();
        };

        climaActvSignal_.subscribe([this, handleSignals] {
            std::lock_guard<Mutex> lock(mutex_);
            handleSignals();
        });

        vehicleModeSignal_.subscribe([this, handleSignals] {
            std::lock_guard<Mutex> lock(mutex_);
            handleSignals();
        });
    }
    else
    {
        ionizer_.set(FirstRowGen::IonizerState::NOT_PRESENT);
        airClngReqSignal_.send(autosar::OnOffNoReq::NoReq);

        log_warning() << LOG_PREFIX << "Not present by car config";
    }

    log_debug() << LOG_PREFIX << "Ionizer running";
}

bool IonizerLogic::signalsOk() const
{
    return vehicleModeSignal_.get().isOk() && climaActvSignal_.get().isOk();
}

void IonizerLogic::activationCheck()
{
    isOk_ = signalsOk();

    if (isOk_)
    {
        auto const usageMode = vehicleModeSignal_.get().value().UsgModSts;
        auto const carMode   = vehicleModeSignal_.get().value().CarModSts1_;

        auto const carModeOk
            = (carMode == autosar::CarModSts1::CarModNorm) || (carMode == autosar::CarModSts1::CarModDyno);

        auto const usageModeOk = (usageMode == autosar::UsgModSts1::UsgModDrvg
                                  || (usageMode != autosar::UsgModSts1::UsgModAbdnd
                                      && climaActvSignal_.get().value() == autosar::OnOff1::On));

        auto const active = usageModeOk && carModeOk;

        log_debug() << LOG_PREFIX << "checked active: " << active;
        isActive_ = active;
    }
}

CommonTypesGen::ReturnCode IonizerLogic::request(FirstRowGen::IonizerRequest const request)
{
    log_debug() << LOG_PREFIX << "request: " << static_cast<int>(request);

    switch (ionizer_.get())
    {
    case FirstRowGen::IonizerState::NOT_PRESENT:
        return CommonTypesGen::ReturnCode::FUNCTION_NOT_PRESENT;
    case FirstRowGen::IonizerState::DISABLED:
        return CommonTypesGen::ReturnCode::FUNCTION_IS_DISABLED;
    case FirstRowGen::IonizerState::SYSTEM_ERROR:
        return CommonTypesGen::ReturnCode::NOT_READY;
    default:
        break;
    }

    switch (request)
    {
    case FirstRowGen::IonizerRequest::OFF:
        ionizerSetting_.set(false);
        break;
    case FirstRowGen::IonizerRequest::ON:
        ionizerSetting_.set(true);
        break;
    default:
        break;
    }

    return CommonTypesGen::ReturnCode::SUCCESS;
}

void IonizerLogic::update()
{
    auto state = ionizer_.get();
    log_debug() << LOG_PREFIX << "isOk: " << isOk_ << " isActive: " << isActive_;

    if (!isOk_)
    {
        state = FirstRowGen::IonizerState::SYSTEM_ERROR;
        airClngReqSignal_.send(autosar::OnOffNoReq::NoReq);
    }
    else
    {
        if (isActive_)
        {
            if (ionizerSetting_.get())
            {
                state = FirstRowGen::IonizerState::ON;
                airClngReqSignal_.send(autosar::OnOffNoReq::On);
            }
            else
            {
                state = FirstRowGen::IonizerState::OFF;
                airClngReqSignal_.send(autosar::OnOffNoReq::Off);
            }
        }
        else
        {
            state = FirstRowGen::IonizerState::DISABLED;
            airClngReqSignal_.send(autosar::OnOffNoReq::NoReq);
        }
    }

    log_debug() << LOG_PREFIX << "updated state: " << state;
    ionizer_.set(state);
}
}
