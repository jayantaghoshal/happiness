/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_sync.h"

#include "first_row_printers.h"
#include "logging.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

namespace {
auto const LOG_PREFIX = "TemperatureSyncLogic: ";

bool carConfigOk() {
    try {
        using CC175 = CarConfigParams::CC175_HvacVariantsType;

        auto const carConfigValue = carconfig::getValue<CC175>();

        return carConfigValue == CC175::Electronic_Climate_Control_2_zone ||
               carConfigValue == CC175::Electronic_Climate_Control_4_zone || carConfigValue == CC175::HVAC_Small_2_zone;
    } catch (std::out_of_range const& e) {
        log_fatal() << LOG_PREFIX << "CarConfig threw: " << e.what();
        throw;
    }
}
}  // namespace

TemperatureSyncLogic::TemperatureSyncLogic(
        NotifiableProperty<FirstRowGen::TemperatureSyncState>& temperatureSync,
        ReadOnlyNotifiableProperty<double>& driverTemp,
        ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& driverTempHiLoN,
        ReadOnlyNotifiableProperty<double>& passengerTemp,
        ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& passengerTempHiLoN,
        ReadOnlyNotifiableProperty<double>& rearLeftTemp,
        ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& rearLeftTempHiLoN,
        ReadOnlyNotifiableProperty<double>& rearRightTemp,
        ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>& rearRightTempHiLoN,
        ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
        std::unique_ptr<SettingsProxy<bool, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER>>
                temperatureSyncSetting)
    : temperatureSync_{temperatureSync},
      driverTemp_{driverTemp},
      driverTempHiLoN_{driverTempHiLoN},
      passengerTemp_{passengerTemp},
      passengerTempHiLoN_{passengerTempHiLoN},
      rearLeftTemp_{rearLeftTemp},
      rearLeftTempHiLoN_{rearLeftTempHiLoN},
      rearRightTemp_{rearRightTemp},
      rearRightTempHiLoN_{rearRightTempHiLoN},
      maxDefroster_{maxDefroster},
      temperatureSyncSetting_{std::move(temperatureSyncSetting)},
      temperatureSyncSettingGETPORT_{temperatureSyncSetting_->defaultValuePORTHELPER()},
      vehicleModeSignal_{},
      climaActvSignal_{},
      carConfigOk_{},
      mutex_{},
      subscriptions_{} {
    carConfigOk_ = carConfigOk();
    if (carConfigOk_) {
        vehicleModeSignal_.subscribe([this] {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            updateState();
        });

        climaActvSignal_.subscribe([this] {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            updateState();
        });

        subscriptions_.push_back(maxDefroster_.subscribe([this](const auto&) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            this->updateState();
        }));

        temperatureSyncSetting_->subscribe([this](auto newSetting) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            temperatureSyncSettingGETPORT_ = newSetting;
            updateState();
        });

        const auto handlePassengerTemp = [this](const auto temp) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            if (temperatureSync_.get() == FirstRowGen::TemperatureSyncState::ON && temp != driverTemp_.get()) {
                log_debug() << LOG_PREFIX << "Set temp sync to OFF";
                temperatureSync_.set(FirstRowGen::TemperatureSyncState::OFF);
            }
        };

        const auto handlePassengerTempHiLoN = [this](const auto tempHiLoN) {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
            if (temperatureSync_.get() == FirstRowGen::TemperatureSyncState::ON &&
                tempHiLoN != driverTempHiLoN_.get()) {
                log_debug() << LOG_PREFIX << "Set temp sync to OFF";
                temperatureSync_.set(FirstRowGen::TemperatureSyncState::OFF);
            }
        };

        subscriptions_.push_back(passengerTemp_.subscribe(handlePassengerTemp));
        subscriptions_.push_back(passengerTempHiLoN_.subscribe(handlePassengerTempHiLoN));
        subscriptions_.push_back(rearLeftTemp_.subscribe(handlePassengerTemp));
        subscriptions_.push_back(rearLeftTempHiLoN_.subscribe(handlePassengerTempHiLoN));
        subscriptions_.push_back(rearRightTemp_.subscribe(handlePassengerTemp));
        subscriptions_.push_back(rearRightTempHiLoN_.subscribe(handlePassengerTempHiLoN));
    } else {
        log_warning() << LOG_PREFIX << "Not present by car config";
        temperatureSync_.set(FirstRowGen::TemperatureSyncState::NOT_PRESENT);
    }
}

void TemperatureSyncLogic::updateState() {
    if (isActive()) {
        if (temperatureSyncSettingGETPORT_) {
            temperatureSync_.set(FirstRowGen::TemperatureSyncState::ON);
        } else {
            temperatureSync_.set(FirstRowGen::TemperatureSyncState::OFF);
        }
    } else {
        temperatureSync_.set(FirstRowGen::TemperatureSyncState::DISABLED);
    }

    log_debug() << LOG_PREFIX << "Set temp sync to " << temperatureSync_.get();
}

CommonTypesGen::ReturnCode TemperatureSyncLogic::request(FirstRowGen::TemperatureSyncRequest temperatureSyncRequest) {
    if (carConfigOk_) {
        if (isActive()) {
            if (temperatureSyncRequest == FirstRowGen::TemperatureSyncRequest::ON) {
                temperatureSync_.set(FirstRowGen::TemperatureSyncState::ON);
                temperatureSyncSetting_->set(true);
            } else {
                temperatureSync_.set(FirstRowGen::TemperatureSyncState::OFF);
                temperatureSyncSetting_->set(false);
            }
            return CommonTypesGen::ReturnCode::SUCCESS;
        } else {
            temperatureSync_.set(FirstRowGen::TemperatureSyncState::DISABLED);
            return CommonTypesGen::ReturnCode::FUNCTION_IS_DISABLED;
        }
    } else {
        temperatureSync_.set(FirstRowGen::TemperatureSyncState::NOT_PRESENT);
        return CommonTypesGen::ReturnCode::FUNCTION_NOT_PRESENT;
    }

    log_debug() << LOG_PREFIX << "Request temp sync to " << temperatureSync_.get();
}

bool TemperatureSyncLogic::isActive() {
    if (vehicleModeSignal_.get().isOk() && climaActvSignal_.get().isOk() &&
        maxDefroster_.get().value_ != FirstRowGen::MaxDefrosterState::ON) {
        auto usgModSts = vehicleModeSignal_.get().value().UsgModSts;
        auto carModSts = vehicleModeSignal_.get().value().CarModSts1_;
        auto climaActv = climaActvSignal_.get().value();

        return (usgModSts == autosar::UsgModSts1::UsgModDrvg ||
                (climaActv == autosar::OnOff1::On && usgModSts != autosar::UsgModSts1::UsgModAbdnd)) &&
               (carModSts == autosar::CarModSts1::CarModNorm || carModSts == autosar::CarModSts1::CarModDyno);
    } else {
        return false;
    }
}
