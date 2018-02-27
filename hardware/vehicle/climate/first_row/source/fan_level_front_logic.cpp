/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "fan_level_front_logic.h"

#include "autosar_printers.h"
#include "first_row_printers.h"
#include "kpi_log.h"
#include "local_config_helper.h"
#include "logging_context.h"
#include "settings_proxy.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace SettingsFramework;
using namespace ApplicationDataElement;
using namespace std::chrono_literals;

namespace {

auto const LOG_PREFIX = "FanLevelFrontLogic: ";

autosar::HmiHvacFanLvl toMccAutosar(const FirstRowGen::FanLevelFrontValue level) {
    switch (level) {
        case FirstRowGen::FanLevelFrontValue::OFF:
            return autosar::HmiHvacFanLvl::Off;
        case FirstRowGen::FanLevelFrontValue::LVL1:
            return autosar::HmiHvacFanLvl::LvlMan1;
        case FirstRowGen::FanLevelFrontValue::LVL2:
            return autosar::HmiHvacFanLvl::LvlMan2;
        case FirstRowGen::FanLevelFrontValue::LVL3:
            return autosar::HmiHvacFanLvl::LvlMan3;
        case FirstRowGen::FanLevelFrontValue::LVL4:
            return autosar::HmiHvacFanLvl::LvlMan4;
        case FirstRowGen::FanLevelFrontValue::LVL5:
            return autosar::HmiHvacFanLvl::LvlMan5;
        case FirstRowGen::FanLevelFrontValue::MAX:
            return autosar::HmiHvacFanLvl::Max;
        default:
            return autosar::HmiHvacFanLvl::Off;
    }
}

autosar::HmiHvacFanLvl toEccAutosar(const FirstRowGen::FanLevelFrontValue level) {
    switch (level) {
        case FirstRowGen::FanLevelFrontValue::OFF:
            return autosar::HmiHvacFanLvl::Off;
        case FirstRowGen::FanLevelFrontValue::LVL1:
            return autosar::HmiHvacFanLvl::LvlAutMinusMinus;
        case FirstRowGen::FanLevelFrontValue::LVL2:
            return autosar::HmiHvacFanLvl::LvlAutMinus;
        case FirstRowGen::FanLevelFrontValue::LVL3:
            return autosar::HmiHvacFanLvl::LvlAutoNorm;
        case FirstRowGen::FanLevelFrontValue::LVL4:
            return autosar::HmiHvacFanLvl::LvlAutPlus;
        case FirstRowGen::FanLevelFrontValue::LVL5:
            return autosar::HmiHvacFanLvl::LvlAutPlusPlus;
        case FirstRowGen::FanLevelFrontValue::MAX:
            return autosar::HmiHvacFanLvl::Max;
        default:
            return autosar::HmiHvacFanLvl::Off;
    }
}
}  // namespace

FanLevelFrontLogic::FanLevelFrontLogic(
        SettingsProxyInterface<FirstRowGen::FanLevelFrontValue::Literal>& autoFanLevelSetting,
        SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, SettingsFramework::UserScope::USER,
                      SettingsFramework::UserScope::NOT_USER_RELATED>& fanLevelFrontSetting,
        NotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
        ReadOnlyNotifiableProperty<FirstRowGen::FirstRow::MaxDefrosterState>& maxDefroster,
        ReadOnlyNotifiableProperty<FirstRowGen::FirstRow::AutoClimateState>& autoClimate,
        ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset, IDispatcher& timerDispatcher)
    : autoFanLevelSetting_{autoFanLevelSetting},
      fanLevelFrontSetting_{fanLevelFrontSetting},
      fanLevelFront_{fanLevelFront},
      maxDefroster_{maxDefroster},
      autoClimate_{autoClimate},
      climateReset_{climateReset},
      timerDispatcher_{timerDispatcher},
      mutex_{},
      subscriptions_{},
      timeout_{util::readLocalConfig<std::chrono::seconds>("LCFG_FanLevelFront_timeout")},
      climateControl_{NONE},
      reqFanLevel_{FirstRowGen::FanLevelFrontValue::OFF},
      currentFanLevel_{FirstRowGen::FanLevelFrontValue::OFF},
      reqAutoClimate_{FirstRowGen::AutoClimateState::NOT_VISIBLE},
      currentAutoClimate_{FirstRowGen::AutoClimateState::NOT_VISIBLE},
      reqMaxDefrost_{FirstRowGen::MaxDefrosterState::OFF},
      currentMaxDefrost_{FirstRowGen::MaxDefrosterState::OFF},
      usageMode_{autosar::UsgModSts1::UsgModAbdnd},
      carMode_{autosar::CarModSts1::CarModFcy},
      climateActive_{autosar::OnOff1::Off} {
    registerFsm();

    if (carConfigState(climateControl_)) {
        executeTransition(FanLevelFrontState::INIT, FanLevelFrontState::SYSTEM_OK);

        fanLevelFrontSetting_.subscribe([this]() {
            log_debug() << "FanLevelFrontLogic: Setting updated";
            reqFanLevel_ = fanLevelFrontSetting_.get();
            changeFanLevelCheck();
        });

        subscriptions_.push_back(maxDefroster_.subscribe([this](const auto) { this->handleMaxDefroster(); }));
        subscriptions_.push_back(autoClimate_.subscribe([this](const auto) { this->handleAutoClimate(); }));
        subscriptions_.push_back(climateReset_.subscribe([this](const auto) {
            if (climateReset_.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) {
                log_debug() << "FanLevelFrontLogic: Handling climate reset";
                this->resetFanLevel(FirstRowGen::FanLevelFrontValue::LVL3);
            }
        }));

        vehicleModesSignal_.subscribe([this]() { handleUsageAndCarMode(); });
        climateActiveSignal_.subscribe([this]() { handleClimateActive(); });

        reqFanLevel_ = fanLevelFrontSetting_.get();
        changeFanLevelCheck();
    } else {
        executeTransition(FanLevelFrontState::INIT, FanLevelFrontState::CARCONFIG_INVALID);
        log_error() << LOG_PREFIX << " No support!";
    }
}

bool FanLevelFrontLogic::registerFsm() {
    // Setup Main FSM
    createState(FanLevelFrontState::INIT);
    registerMainInitState(INIT);

    // Create all states
    createState(FanLevelFrontState::CARCONFIG_INVALID);
    createState(FanLevelFrontState::SYSTEM_OK);
    createState(FanLevelFrontState::SYSTEM_ERROR);
    createState(FanLevelFrontState::NOT_ACTIVE, FanLevelFrontState::SYSTEM_OK);
    createState(FanLevelFrontState::ACTIVE, FanLevelFrontState::SYSTEM_OK);
    createState(FanLevelFrontState::ON, FanLevelFrontState::ACTIVE);
    createState(FanLevelFrontState::OFF, FanLevelFrontState::ACTIVE);

    // Add entry, exit and body
    addStateEntry(FanLevelFrontState::CARCONFIG_INVALID,
                  [this]() { setFanLevel(FirstRowGen::FanLevelFrontValue::DISABLED); });
    addStateEntry(FanLevelFrontState::SYSTEM_ERROR,
                  [this]() { setFanLevel(FirstRowGen::FanLevelFrontValue::SYSTEM_ERROR); });
    addStateEntry(FanLevelFrontState::OFF, [this]() { setFanLevel(FirstRowGen::FanLevelFrontValue::OFF); });
    addStateEntry(FanLevelFrontState::NOT_ACTIVE, [this]() { setFanLevel(FirstRowGen::FanLevelFrontValue::DISABLED); });
    addStateEntry(FanLevelFrontState::ACTIVE, [this]() { transitionToState(activeEntry()); });
    addStateEntry(FanLevelFrontState::ON, [this]() {
        if (currentMaxDefrost_ == FirstRowGen::MaxDefrosterState::ON) {
            setFanLevel(FirstRowGen::FanLevelFrontValue::LVL5);
        } else if (climateActive_ == autosar::OnOff1::On) {
            setFanLevel(autoFanLevelSetting_.get());
        } else {
            setFanLevel(fanLevelFrontSetting_.get());
        }
    });

    // Add init states within states
    addInitState(FanLevelFrontState::SYSTEM_OK, FanLevelFrontState::NOT_ACTIVE);

    // Create transitions between states->
    createTransition(FanLevelFrontState::INIT, FanLevelFrontState::CARCONFIG_INVALID);
    createTransition(FanLevelFrontState::INIT, FanLevelFrontState::SYSTEM_OK);
    createTransition(FanLevelFrontState::ACTIVE, FanLevelFrontState::NOT_ACTIVE,
                     [this]() { return !activationCheck(); });
    createTransition(FanLevelFrontState::NOT_ACTIVE, FanLevelFrontState::ACTIVE,
                     [this]() { return activationCheck(); });
    createTransition(FanLevelFrontState::ACTIVE, FanLevelFrontState::ON);
    createTransition(FanLevelFrontState::ACTIVE, FanLevelFrontState::OFF);
    createTransition(FanLevelFrontState::OFF, FanLevelFrontState::ON);
    createTransition(FanLevelFrontState::ON, FanLevelFrontState::OFF);
    createTransition(FanLevelFrontState::SYSTEM_OK, FanLevelFrontState::SYSTEM_ERROR);
    createTransition(FanLevelFrontState::SYSTEM_ERROR, FanLevelFrontState::SYSTEM_OK);

    return true;
}

bool FanLevelFrontLogic::carConfigState(ClimateControl& climateControl) {
    using CC175 = CarConfigParams::CC175_HvacVariantsType;

    try {
        auto result = false;

        const auto hvacVariant = carconfig::getValue<CC175>();

        switch (hvacVariant) {
            case CC175::Electronic_Climate_Control_2_zone:
            case CC175::Electronic_Climate_Control_4_zone:
            case CC175::HVAC_Small_1_zone:
            case CC175::HVAC_Small_2_zone:
            case CC175::HVAC_Small_3_zone:
                climateControl = ClimateControl::ECC;
                result = true;
                break;
            case CC175::Manual_AC:
                climateControl = ClimateControl::MCC;
                result = true;
                break;
            default:
                climateControl = ClimateControl::NONE;
                result = false;
                break;
        }

        return result;
    } catch (const std::out_of_range& e) {
        log_error() << LOG_PREFIX << "CarConfig threw: " << e.what();
        throw;
    }
}

bool FanLevelFrontLogic::isInState(int state) { return inState(state); }

bool FanLevelFrontLogic::activationCheck() const {
    return ((carMode_ == autosar::CarModSts1::CarModDyno || carMode_ == autosar::CarModSts1::CarModNorm) &&
            ((usageMode_ != autosar::UsgModSts1::UsgModInActv && usageMode_ != autosar::UsgModSts1::UsgModAbdnd) ||
             climateActive_ == autosar::OnOff1::On));
}

FanLevelFrontLogic::FanLevelFrontState FanLevelFrontLogic::activeEntry() const {
    if (((usageMode_ == autosar::UsgModSts1::UsgModCnvinc || usageMode_ == autosar::UsgModSts1::UsgModActv) &&
         (climateActive_ == autosar::OnOff1::Off))) {
        return FanLevelFrontState::OFF;
    } else {
        return FanLevelFrontState::ON;
    }
}

bool FanLevelFrontLogic::changeFanLevelCheck() {
    if (inState(FanLevelFrontState::ACTIVE)) {
        if (reqFanLevel_ != currentFanLevel_) {
            if (currentMaxDefrost_ != FirstRowGen::MaxDefrosterState::ON) {
                if (reqFanLevel_ != FirstRowGen::FanLevelFrontValue::MAX &&
                    reqFanLevel_ != FirstRowGen::FanLevelFrontValue::OFF) {
                    setAutoFanLevel(reqFanLevel_);
                    autoFanLevelSetting_.set(
                            static_cast<FirstRowGen::FanLevelFrontValue::Literal>(reqFanLevel_.value_));
                }
                fanLevelFrontSetting_.set(static_cast<FirstRowGen::FanLevelFrontValue::Literal>(reqFanLevel_.value_));
            }
            setFanLevel(reqFanLevel_);
            log_debug() << LOG_PREFIX << "New fan level requested, set level to : " << reqFanLevel_;
        } else if (reqAutoClimate_ != currentAutoClimate_ && reqAutoClimate_ == FirstRowGen::AutoClimateState::ON) {
            auto const value = autoFanLevelSetting_.get();
            setFanLevel(value);
            fanLevelFrontSetting_.set(value);
            log_debug() << LOG_PREFIX << "Handle Auto climate, set level to : " << value;
            currentAutoClimate_ = reqAutoClimate_;
        } else if (reqMaxDefrost_ != currentMaxDefrost_) {
            if (reqMaxDefrost_ == FirstRowGen::MaxDefrosterState::ON &&
                currentFanLevel_ != FirstRowGen::FanLevelFrontValue::LVL5) {
                setFanLevel(FirstRowGen::FanLevelFrontValue::LVL5);
                log_debug() << LOG_PREFIX
                            << "Handle max defroster, set level to : " << FirstRowGen::FanLevelFrontValue::LVL5;
            } else if (currentMaxDefrost_ == FirstRowGen::MaxDefrosterState::ON &&
                       currentAutoClimate_ != FirstRowGen::AutoClimateState::ON) {
                if (FirstRowGen::AutoClimateState::ON == autoClimate_.get()) {
                    auto value = autoFanLevelSetting_.get();
                    setFanLevel(value);
                    log_debug() << LOG_PREFIX << "Handle max defroster (auto climate on), set level to : " << value;
                } else {
                    auto value = fanLevelFrontSetting_.get();
                    setFanLevel(value);
                    log_debug() << LOG_PREFIX << "Handle max defroster, set level to : " << value;
                }
            }
        }

        currentMaxDefrost_ = reqMaxDefrost_;

        // Request for Fan Level front expired
        reqFanLevel_ = currentFanLevel_;

        // When AutoClimate is set to OFF we have to save the last value.
        currentAutoClimate_ = reqAutoClimate_;

        if (FirstRowGen::FanLevelFrontValue::OFF == reqFanLevel_) {
            // This transition is guarded.
            transitionToState(FanLevelFrontState::OFF);
        } else if (inState(FanLevelFrontState::OFF)) {
            transitionToState(FanLevelFrontState::ON);
        }

        if (inState(FanLevelFrontState::ON)) {
            log_debug() << LOG_PREFIX << "Start timer!!!";

            timerDispatcher_.Start(timeout_, [this] {
                log_debug() << LOG_PREFIX << "Timeout";

                if (((usageMode_ == autosar::UsgModSts1::UsgModCnvinc) ||
                     (usageMode_ == autosar::UsgModSts1::UsgModActv)) &&
                    (climateActive_ == autosar::OnOff1::Off)) {
                    transitionToState(FanLevelFrontState::OFF);
                }
            });
        } else {
            timerDispatcher_.Cancel();
        }

        log_debug() << LOG_PREFIX << "FanLevel: " << reqFanLevel_;
        return true;
    } else {
        log_debug() << LOG_PREFIX << "Not Active!";
    }

    return false;
}

void FanLevelFrontLogic::handleMaxDefroster() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    reqMaxDefrost_ = maxDefroster_.get();

    log_debug() << LOG_PREFIX << "handleMaxDefroster : " << reqMaxDefrost_;
    changeFanLevelCheck();
}

void FanLevelFrontLogic::handleAutoClimate() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    reqAutoClimate_ = autoClimate_.get();

    log_debug() << LOG_PREFIX << "handleAutoClimate : " << reqAutoClimate_;
    changeFanLevelCheck();
}

void FanLevelFrontLogic::handleUsageAndCarMode() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    usageMode_ = vehicleModesSignal_.get().value().UsgModSts;
    carMode_ = vehicleModesSignal_.get().value().CarModSts1_;

    if (activationCheck()) {
        log_debug() << LOG_PREFIX << "handleUsageAndCarMode  " << usageMode_ << ", " << carMode_ << ", in state active";
        transitionToState(FanLevelFrontState::ACTIVE);

        if (inState(FanLevelFrontState::ACTIVE) && usageMode_ == autosar::UsgModSts1::UsgModDrvg &&
            carMode_ != autosar::CarModSts1::CarModDyno) {
            executeTransition(FanLevelFrontState::OFF, FanLevelFrontState::ON);
        }

        changeFanLevelCheck();
    } else {
        log_debug() << LOG_PREFIX << "In state not active";
        transitionToState(FanLevelFrontState::NOT_ACTIVE);
    }
}

void FanLevelFrontLogic::handleClimateActive() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    auto const prevClimateActive = climateActive_;
    climateActive_ = climateActiveSignal_.get().value();

    if (climateActiveSignal_.get().isError()) {
        transitionToState(FanLevelFrontState::SYSTEM_ERROR);
        log_debug() << LOG_PREFIX << "handleClimateActive Signal Error!";
    } else if (climateActiveSignal_.get().isOk()) {
        if (inState(FanLevelFrontState::SYSTEM_ERROR)) {
            transitionToState(FanLevelFrontState::SYSTEM_OK);
            if (activationCheck()) {
                transitionToState(FanLevelFrontState::ACTIVE);
                reqFanLevel_ = fanLevelFrontSetting_.get();
                changeFanLevelCheck();
            } else {
                transitionToState(FanLevelFrontState::NOT_ACTIVE);
            }
        } else if (inState(FanLevelFrontState::SYSTEM_OK)) {
            if (inState(FanLevelFrontState::NOT_ACTIVE) && activationCheck()) {
                transitionToState(FanLevelFrontState::ACTIVE);
                reqFanLevel_ = fanLevelFrontSetting_.get();
                changeFanLevelCheck();
            }
            {
                transitionToState(FanLevelFrontState::NOT_ACTIVE);

                if (inState(FanLevelFrontState::ACTIVE)) {
                    if (prevClimateActive == autosar::OnOff1::On && climateActive_ == autosar::OnOff1::Off) {
                        executeTransition(FanLevelFrontState::ON, FanLevelFrontState::OFF);
                    }
                }
            }
        }
    }
}

void FanLevelFrontLogic::sendFanLevel(FirstRowGen::FanLevelFrontValue level) {
    auto fanLevel = autosar::HmiHvacFanLvl::Off;

    if (climateControl_ == ClimateControl::MCC) {
        fanLevel = toMccAutosar(level);
    } else if (climateControl_ == ClimateControl::ECC) {
        fanLevel = toEccAutosar(level);
    }

    fanLevelFrontSignal_.send(fanLevel);
    log_info() << KPI_MARKER << LOG_PREFIX << "FanLevel is " << static_cast<int>(fanLevel);
}

void FanLevelFrontLogic::setFanLevel(FirstRowGen::FanLevelFrontValue value) {
    fanLevelFront_.set(value);
    currentFanLevel_ = value;
    reqFanLevel_ = value;

    sendFanLevel(value);
}

void FanLevelFrontLogic::setAutoFanLevel(FirstRowGen::FanLevelFrontValue value) {
    autoFanLevelSetting_.set(static_cast<FirstRowGen::FanLevelFrontValue::Literal>(value.value_));
}

void FanLevelFrontLogic::requestFanLevel(FirstRowGen::FanLevelFrontRequest fanLevel) {
    // This conversion might be dangerous, the intention is to convert from FanLevelFrontRequest to FanLevelFrontValue
    reqFanLevel_ = static_cast<FirstRowGen::FanLevelFrontValue::Literal>(fanLevel.value_);
    changeFanLevelCheck();
}
void FanLevelFrontLogic::resetFanLevel(FirstRowGen::FanLevelFrontValue level) {
    reqFanLevel_ = level;
    currentFanLevel_ = level;

    fanLevelFrontSetting_.set(static_cast<FirstRowGen::FanLevelFrontValue::Literal>(level.value_));

    setAutoFanLevel(level);
    setFanLevel(level);
    log_debug() << LOG_PREFIX << "Set fan level: " << level;
}
