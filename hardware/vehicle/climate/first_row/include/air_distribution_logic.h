/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef CLIMATE_DAEMON_AIRDISTRIBUTION_LOGIC_H_
#define CLIMATE_DAEMON_AIRDISTRIBUTION_LOGIC_H_

#include <Application_dataelement.h>
#include <libsettings/setting.h>
#include <settings_proxy.h>
#include <mutex>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include "auto_climate_logic.h"
#include "climate_reset_logic.h"
#include "fan_level_front_logic.h"
#include "max_defroster_logic.h"

class AirDistributionLogic {
    using FirstRowGen = v0::org::volvocars::climate::FirstRow;
    using AutoClimateState = FirstRowGen::AutoClimateState;
    using AirDistributionAngle = FirstRowGen::AirDistributionAngle;
    using AirDistributionAngleRequest = FirstRowGen::AirDistributionAngleRequest;

  public:
    explicit AirDistributionLogic(
            NotifiableProperty<AirDistributionAngle>& AirDistribution,
            SettingsProxy<FirstRowGen::AirDistributionAngle::Literal, SettingsFramework::UserScope::USER,
                          SettingsFramework::UserScope::NOT_USER_RELATED>& settingAirDistribution,
            ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset,
            ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevel,
            ReadOnlyNotifiableProperty<AutoClimateState>& autoClimate,
            ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster);
    ~AirDistributionLogic();

    void request(AirDistributionAngleRequest value);

  public:
    enum class ClimateControl { NONE, ECC, MCC };

  private:
    bool activationCheck() const;

    bool notSettingAutoWithMcc(AirDistributionAngleRequest value) const;
    bool carModeIsDyno() const;

    void handleSignals();
    void setStateAndValue(bool isActive, FirstRowGen::FanLevelFrontValue fanLevel,
                          AirDistributionAngle airDistributionAngle);

  private:
    ClimateControl const climateControl_;
    SubscriptionHandle airDistributionSubscriptionId_;
    SubscriptionHandle autoClimateSubscriptionId_;
    SubscriptionHandle fanLevelSubscriptionId_;
    SubscriptionHandle maxDefrosterSubscriptionId_;
    SubscriptionHandle climateResetSubscriptionId_;

    SettingsProxy<FirstRowGen::AirDistributionAngle::Literal, SettingsFramework::UserScope::USER,
                  SettingsFramework::UserScope::NOT_USER_RELATED>& sAirDistribution;

    NotifiableProperty<AirDistributionAngle>& value_;
    ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& shareFanLevelFront;
    ReadOnlyNotifiableProperty<AutoClimateState>& shareAutoClimate;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& shareMaxDefroster;
    ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& shareClimateReset;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1_;
    ApplicationDataElement::DESender<autosar::HmiCmptmtAirDistbnFrnt_info> hmiCmptmtAirDistbnFrnt_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActive_;

    std::recursive_mutex mutex_;

    bool isActive_;
    bool carModeReceived;
    bool maxDefrosterOn_;
};

#endif  // CLIMATE_DAEMON_AIRDISTRIBUTION_LOGIC_H_
