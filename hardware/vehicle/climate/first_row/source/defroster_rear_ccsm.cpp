/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "defroster_rear_ccsm.h"

#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

namespace
{
auto LOG_PREFIX = "DefrosterWindscreenCCSM: ";
}

DefrosterRearCCSM::DefrosterRearCCSM(ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& rearDefroster,
                                     IDefroster& electricDefrosterRearLogic,
                                     IDefroster& electricDefrosterRearPopupLogic)
    : rearDefroster_{ rearDefroster }
    , subscriptions_{}
    , electricDefrosterRearLogic_{ electricDefrosterRearLogic }
    , electricDefrosterRearPopupLogic_{ electricDefrosterRearPopupLogic }
{
    defrosterRearButtonReq_.subscribe([this]() { updateRearDefroster(); });
    subscriptions_.push_back(rearDefroster_.subscribe([this](const auto&) { this->updateButtonLed(); }));
}

void DefrosterRearCCSM::updateButtonLed()
{
    if (rearDefroster_.get() == FirstRowGen::ElectricDefrosterRearState::ON)
    {
        log_debug() << LOG_PREFIX << "LED ON";
        rearDefrosterButtonLedReq_.send(autosar::OnOff1::On);
    }
    else
    {
        log_debug() << LOG_PREFIX << "LED OFF";
        rearDefrosterButtonLedReq_.send(autosar::OnOff1::Off);
    }
}

void DefrosterRearCCSM::updateRearDefroster()
{
    if (defrosterRearButtonReq_.get().value() != autosar::PsdNotPsd::Psd)
    {
        return;
    }

    if (rearDefroster_.get() == FirstRowGen::ElectricDefrosterRearState::ON)
    {
        log_debug() << LOG_PREFIX << "Electric Defroster Rear requested Off";
        electricDefrosterRearLogic_.request(OnOff::Off);
        electricDefrosterRearPopupLogic_.request(OnOff::Off);
    }
    else
    {
        log_debug() << LOG_PREFIX << "Electric Defroster Rear requested On";
        electricDefrosterRearLogic_.request(OnOff::On);
        electricDefrosterRearPopupLogic_.request(OnOff::On);
    }
}
