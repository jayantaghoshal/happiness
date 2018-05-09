/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_synchronizer.h"

#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

namespace common {
namespace daemon {

TemperatureSynchronizer::TemperatureSynchronizer(
        common::daemon::ITemperatureLogic& passengerTemperatureLogic,
        ReadOnlyNotifiableProperty<FirstRowGen::TemperatureSyncState>& temperatureSync,
        ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
        ReadOnlyNotifiableProperty<double>& driverTemp,
        ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& driverTempHiLoN)
    : passengerTemperatureLogic_{passengerTemperatureLogic},
      temperatureSync_{temperatureSync},
      maxDefroster_{maxDefroster},
      driverTemp_{driverTemp},
      driverTempHiLoN_{driverTempHiLoN},
      subscriptions_{} {
    subscriptions_.push_back(temperatureSync_.subscribe(
            [this](const auto&) { this->synchronize(driverTemp_.get(), driverTempHiLoN_.get()); }));

    subscriptions_.push_back(
            driverTemp_.subscribe([this](const auto temp) { this->synchronize(temp, driverTempHiLoN_.get()); }));

    subscriptions_.push_back(driverTempHiLoN_.subscribe(
            [this](const auto tempHiLoN) { this->synchronize(driverTemp_.get(), tempHiLoN); }));

    // Initialization
    synchronize(driverTemp_.get(), driverTempHiLoN_.get());
}

void TemperatureSynchronizer::synchronize(double temp, autosar::HmiCmptmtTSpSpcl tempHiLoN) {
    if ((temperatureSync_.get() == FirstRowGen::TemperatureSyncState::ON) &&
        (maxDefroster_.get() != FirstRowGen::MaxDefrosterState::ON)) {
        log_debug() << "TemperatureSynchronizer, sync is ON and max Defroster Not ON, request temp";
        passengerTemperatureLogic_.request(temp);
        passengerTemperatureLogic_.request(tempHiLoN);
    } else {
        log_debug() << "TemperatureSynchronizer, sync is NOT ON, do noting";
    }
}
}  // namespace daemon
}  // namespace common
