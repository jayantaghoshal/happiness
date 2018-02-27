/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "defroster_windscreen_ccsm.h"

#include "local_config_helper.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace std::chrono_literals;

namespace {

bool carHasDefrosterWindscreen() {
    try {
        auto heatedWindscreen = carconfig::getValue<CarConfigParams::CC122_HeatedWindscreenType>();
        return heatedWindscreen == CarConfigParams::CC122_HeatedWindscreenType::Heated_Frontscreen;
    } catch (std::out_of_range const& e) {
        log_fatal() << "CarConfig threw: " << e.what();
        throw;
    }
}

auto LOG_PREFIX = "DefrosterWindscreenCCSM: ";
}  // namespace

DefrosterWindscreenCCSM::DefrosterWindscreenCCSM(
        ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
        ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& defrosterWindscreen,
        IDispatcher& dispatcher, IDefroster& electricDefrosterWindscreenLogic,
        IDefroster& electricDefrosterWindscreenPopupLogic, IDefroster& maxDefrosterLogic)
    : maxDefroster_{maxDefroster},
      defrosterWindscreen_{defrosterWindscreen},
      dispatcher_{dispatcher},
      dispatcherTimeout_{
              std::chrono::milliseconds{static_cast<int>(util::readLocalConfig<double>("Defroster_delay") * 1000.0)}},
      mutex_{},
      subscriptions_{},
      electricDefrosterWindscreenLogic_{electricDefrosterWindscreenLogic},
      electricDefrosterWindscreenPopupLogic_{electricDefrosterWindscreenPopupLogic},
      maxDefrosterLogic_{maxDefrosterLogic} {
    if (carHasDefrosterWindscreen()) {
        defrosterButtonReq_.subscribe([this]() { setMaxDefrosterAndDefrosterWindscreen(); });
        subscriptions_.push_back(defrosterWindscreen_.subscribe([this](const auto&) { this->updateButtonLed(); }));
    } else {
        defrosterButtonReq_.subscribe([this]() { updateMaxDefroster(); });
    }

    subscriptions_.push_back(maxDefroster_.subscribe([this](const auto&) { this->updateButtonLed(); }));
}

void DefrosterWindscreenCCSM::updateButtonLed() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    if ((maxDefroster_.get() == FirstRowGen::MaxDefrosterState::ON) ||
        (defrosterWindscreen_.get() == FirstRowGen::ElectricDefrosterWindscreenState::ON)) {
        log_debug() << LOG_PREFIX << "LED ON";
        defrosterButtonLedReq_.send(autosar::OnOff1::On);
    } else {
        log_debug() << LOG_PREFIX << "LED OFF";
        defrosterButtonLedReq_.send(autosar::OnOff1::Off);
    }
}

void DefrosterWindscreenCCSM::updateMaxDefroster() {
    if (defrosterButtonReq_.get().value() != autosar::PsdNotPsd::Psd) {
        return;
    }

    if (maxDefroster_.get() == FirstRowGen::MaxDefrosterState::ON) {
        log_debug() << LOG_PREFIX << "Max Defroster requested Off";
        maxDefrosterLogic_.request(OnOff::Off);
    } else {
        log_debug() << LOG_PREFIX << "Max Defroster requested On";
        maxDefrosterLogic_.request(OnOff::On);
    }
}

void DefrosterWindscreenCCSM::setMaxDefrosterAndDefrosterWindscreen() {
    if (defrosterButtonReq_.get().value() != autosar::PsdNotPsd::Psd) {
        return;
    }

    if ((maxDefroster_.get() == FirstRowGen::MaxDefrosterState::OFF) &&
        (defrosterWindscreen_.get() == FirstRowGen::ElectricDefrosterWindscreenState::OFF)) {
        log_debug() << LOG_PREFIX << "Electric Defroster Windscreen requested On";
        electricDefrosterWindscreenLogic_.request(OnOff::On);
        electricDefrosterWindscreenPopupLogic_.request(OnOff::On);
    } else if ((maxDefroster_.get() == FirstRowGen::MaxDefrosterState::OFF) &&
               (defrosterWindscreen_.get() == FirstRowGen::ElectricDefrosterWindscreenState::ON) &&
               !dispatcher_.IsRunning()) {
        log_debug() << "timer started";
        dispatcher_.Start(dispatcherTimeout_, [this]() {
            log_debug() << LOG_PREFIX << "Max Defroster and Electric Defroster Windscreen requested On";
            maxDefrosterLogic_.request(OnOff::On);
            electricDefrosterWindscreenLogic_.request(OnOff::On);
            electricDefrosterWindscreenPopupLogic_.request(OnOff::On);
        });
    } else {
        dispatcher_.Cancel();
        log_debug() << LOG_PREFIX << "Max Defroster and Electric Defroster Windscreen requested Off";
        maxDefrosterLogic_.request(OnOff::Off);
        electricDefrosterWindscreenLogic_.request(OnOff::Off);
        electricDefrosterWindscreenPopupLogic_.request(OnOff::Off);
    }
}
