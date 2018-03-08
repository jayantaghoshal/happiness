/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "air_distribution_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

using namespace CarConfigParams;

namespace {

AirDistributionLogic::ClimateControl getHvacTypeFromCarConfig() {
    try {
        auto hvacVariant = carconfig::getValue<CC175_HvacVariantsType>();
        switch (hvacVariant) {
            default:
                return AirDistributionLogic::ClimateControl::ECC;
            case CC175_HvacVariantsType::Manual_AC:
                return AirDistributionLogic::ClimateControl::MCC;
            case CC175_HvacVariantsType::InvalidValue:
                return AirDistributionLogic::ClimateControl::NONE;
        }
    } catch (std::exception const& e) {
        log_error() << "Error while reading carconfig: " << e.what();
    }
    // Exception indicates something wrong.  Should we assume Manual?
    return AirDistributionLogic::ClimateControl::MCC;
}

autosar::HmiCmptmtAirDistbnFrnt toHmimComponent(FirstRowGen::AirDistributionAngle angle) {
    using Angle = FirstRowGen::AirDistributionAngle;
    switch (angle) {
        case Angle::FLOOR:
            return autosar::HmiCmptmtAirDistbnFrnt::Flr;
        case Angle::VENT:
            return autosar::HmiCmptmtAirDistbnFrnt::Vent;
        case Angle::DEFROST:
            return autosar::HmiCmptmtAirDistbnFrnt::Defrst;
        case Angle::FLOOR_DEFROST:
            return autosar::HmiCmptmtAirDistbnFrnt::FlrDefrst;
        case Angle::FLOOR_VENT:
            return autosar::HmiCmptmtAirDistbnFrnt::FlrVent;
        case Angle::VENT_DEFROST:
            return autosar::HmiCmptmtAirDistbnFrnt::VentDefrst;
        case Angle::FLOOR_VENT_DEFROST:
            return autosar::HmiCmptmtAirDistbnFrnt::FlrVentDefrst;
        case Angle::AUTO:
        case Angle::DISABLED:
            return autosar::HmiCmptmtAirDistbnFrnt::Aut;
    }

    // The following is just to make g++ shut up since the above switch
    // covers all eight potential values
    return autosar::HmiCmptmtAirDistbnFrnt::Aut;
}

bool fanLevelFrontIsOn(FirstRowGen::FanLevelFrontValue fanLevelFront) {
    return (fanLevelFront == FirstRowGen::FanLevelFrontValue::LVL1 ||
            fanLevelFront == FirstRowGen::FanLevelFrontValue::LVL2 ||
            fanLevelFront == FirstRowGen::FanLevelFrontValue::LVL3 ||
            fanLevelFront == FirstRowGen::FanLevelFrontValue::LVL4 ||
            fanLevelFront == FirstRowGen::FanLevelFrontValue::LVL5 ||
            fanLevelFront == FirstRowGen::FanLevelFrontValue::MAX);
}
}  // namespace

AirDistributionLogic::AirDistributionLogic(
        NotifiableProperty<AirDistributionAngle>& AirDistribution,
        SettingsProxy<FirstRowGen::AirDistributionAngle::Literal, SettingsFramework::UserScope::USER,
                      SettingsFramework::UserScope::NOT_USER_RELATED>& settingAirDistribution,
        ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset,
        ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevel,
        ReadOnlyNotifiableProperty<AutoClimateState>& autoClimate,
        ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster)
    : climateControl_{getHvacTypeFromCarConfig()},
      sAirDistribution{settingAirDistribution},
      sAirDistributionGETPORT{sAirDistribution.defaultValuePORTHELPER()},
      value_{AirDistribution},
      shareFanLevelFront(fanLevel),
      shareAutoClimate{autoClimate},
      shareMaxDefroster{maxDefroster},
      shareClimateReset{climateReset},
      isActive_{false},
      maxDefrosterOn_{shareMaxDefroster.get() == FirstRowGen::MaxDefrosterState::ON} {
    airDistributionSubscriptionId_ =
            value_.subscribe([this](const auto&) { hmiCmptmtAirDistbnFrnt_.send(toHmimComponent(value_.get())); });

    VehModMngtGlbSafe1_.subscribe([this]() { handleSignals(); });
    climateActive_.subscribe([this]() { handleSignals(); });
    fanLevelSubscriptionId_ = shareFanLevelFront.subscribe([this](const auto&) { this->handleSignals(); });

    handleSignals();

    climateResetSubscriptionId_ = shareClimateReset.subscribe([this](const auto&) {
        if (shareClimateReset.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) {
            log_debug() << "AirDistributionLogic: Handling climate reset";
            sAirDistribution.set(static_cast<FirstRowGen::AirDistributionAngle::Literal>(AirDistributionAngle::AUTO));
        }
    });

    autoClimateSubscriptionId_ = shareAutoClimate.subscribe([this](const auto&) {
        if (shareAutoClimate.get() == FirstRowGen::AutoClimateState::ON) {
            isActive_ = true;  // Doesn't care about activation check if auto climate goes ON
            log_debug() << "AirDistributionLogic: Handling auto climate on -> setting value: "
                        << AirDistributionAngle::AUTO;

            value_.set(AirDistributionAngle::AUTO);
            sAirDistribution.set(static_cast<FirstRowGen::AirDistributionAngle::Literal>(AirDistributionAngle::AUTO));
        }
    });

    maxDefrosterSubscriptionId_ = shareMaxDefroster.subscribe([this](const auto&) {
        if (shareMaxDefroster.get() == FirstRowGen::MaxDefrosterState::ON) {
            log_debug() << "AirDistributionLogic: Handling max defroster on.";
            maxDefrosterOn_ = true;
            if (value_.get() != AirDistributionAngle::Literal::AUTO) {
                this->setStateAndValue(isActive_, shareFanLevelFront.get(), AirDistributionAngle::AUTO);
            }
        }
        // hasChangedFrom(ON)
        else if (maxDefrosterOn_ && shareMaxDefroster.get() == FirstRowGen::MaxDefrosterState::OFF) {
            maxDefrosterOn_ = false;

            if (shareAutoClimate.get() == FirstRowGen::AutoClimateState::ON) {
                log_debug() << "AirDistributionLogic: Handling max defroster off. AUTO climate on, go to AUTO";
                this->setStateAndValue(isActive_, shareFanLevelFront.get(), AirDistributionAngle::AUTO);
            } else {
                log_debug() << "AirDistributionLogic: Handling max defroster off. Go to saved value";
                this->setStateAndValue(isActive_, shareFanLevelFront.get(),
                                       static_cast<AirDistributionAngle::Literal>(sAirDistributionGETPORT));
            }
        }
    });

    value_.set(static_cast<AirDistributionAngle::Literal>(sAirDistributionGETPORT));

    sAirDistribution.subscribe([this](auto newSetting) {
        log_debug() << "AirDistributionLogic: Setting updated";
        sAirDistributionGETPORT = newSetting;
        request(static_cast<AirDistributionAngleRequest::Literal>(sAirDistributionGETPORT));
    });
}

AirDistributionLogic::~AirDistributionLogic() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    sAirDistribution.unSubscribe();
}

void AirDistributionLogic::request(AirDistributionAngleRequest value) {
    std::lock_guard<std::recursive_mutex> locker(mutex_);
    log_debug() << "AirDistributionLogic::request, isActive: " << isActive_ << ", value: " << value;
    if (isActive_ && fanLevelFrontIsOn(shareFanLevelFront.get()) &&
        (static_cast<AirDistributionAngle::Literal>(value.value_) != value_.get()) && notSettingAutoWithMcc(value)) {
        log_debug() << "AirDistributionLogic: Saving value to setting: " << value;
        sAirDistribution.set(static_cast<FirstRowGen::AirDistributionAngle::Literal>(value.value_));
        value_.set(static_cast<AirDistributionAngle::Literal>(value.value_));
    }
}

bool AirDistributionLogic::notSettingAutoWithMcc(AirDistributionAngleRequest value) const {
    return !((climateControl_ == ClimateControl::MCC) && (value == AirDistributionAngleRequest::AUTO));
}

void AirDistributionLogic::handleSignals() {
    isActive_ = activationCheck();

    AirDistributionAngle angle = value_.get();
    if (isActive_ && fanLevelFrontIsOn(shareFanLevelFront.get())) {
        // If Max Defroster active, then go direct to AUTO, ignore saved setting
        if (shareMaxDefroster.get() == FirstRowGen::MaxDefrosterState::ON) {
            log_debug() << "AirDistributionLogic: transitioning to active, Handling max defroster on";
            maxDefrosterOn_ = true;
            if (value_.get() != AirDistributionAngle::Literal::AUTO) {
                angle = AirDistributionAngle::AUTO;
            }
        } else {
            log_debug() << "AirDistributionLogic: transitioning to active, reading setting";
            angle = static_cast<AirDistributionAngle::Literal>(sAirDistributionGETPORT);
        }
    } else {
        log_debug() << "AirDistributionLogic: transitioning to not_active";
        angle = AirDistributionAngle::DISABLED;
    }

    log_debug() << "AirDistributionLogic, handleSignals, isActive: " << isActive_ << ", angle: " << angle;
    value_.set(angle);
}

void AirDistributionLogic::setStateAndValue(bool isActive, FirstRowGen::FanLevelFrontValue fanLevel,
                                            AirDistributionAngle airDistributionAngle) {
    AirDistributionAngle angle;

    if (isActive) {
        if (fanLevel == FirstRowGen::FanLevelFrontValue::OFF) {
            angle = AirDistributionAngle::DISABLED;
        } else {
            angle = airDistributionAngle;
        }
    } else {
        angle = AirDistributionAngle::DISABLED;
    }

    log_debug() << "AirDistributionLogic, setting angle: " << angle;

    value_.set(angle);
}

bool AirDistributionLogic::activationCheck() const {
    if (VehModMngtGlbSafe1_.get().isOk() && climateActive_.get().isOk()) {
        const auto usgMode = VehModMngtGlbSafe1_.get().value().UsgModSts;
        const auto carMode = VehModMngtGlbSafe1_.get().value().CarModSts1_;
        const auto climateActive = climateActive_.get().value();

        const auto usageModeNotAbandoned = usgMode != autosar::UsgModSts1::UsgModAbdnd;
        const auto usageModeNotInactive = usgMode != autosar::UsgModSts1::UsgModInActv;
        const auto carModeIsNormal = carMode == autosar::CarModSts1::CarModNorm;
        const auto carModeIsDyno = carMode == autosar::CarModSts1::CarModDyno;
        const auto climateIsActive = climateActive == autosar::OnOff1::On;

        return (carModeIsDyno || carModeIsNormal) &&
               ((usageModeNotAbandoned && usageModeNotInactive) || climateIsActive);
    }

    return false;
}
