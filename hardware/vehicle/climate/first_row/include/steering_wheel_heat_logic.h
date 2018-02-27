/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "notifiable_property.h"
#include "timer_manager_interface.h"

#include <Application_dataelement.h>
#include <cc_parameterlist.h>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "notifiable_property.h"
#include "settings_proxy.h"

#include <atomic>
#include <memory>
#include <mutex>

class SteeringWheelHeatLogic {
  private:
    using FirstRowGen = v0::org::volvocars::climate::FirstRow;
    using UserSelectionGen = v0::org::volvocars::climate::UserSelection;

  public:
    SteeringWheelHeatLogic(NotifiableProperty<FirstRowGen::HeatAttribute>& heatAttribute,
                           ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoSteeringWheelHeatOn,
                           ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoSteeringWheelHeatLevel,
                           SettingsProxyInterface<FirstRowGen::HeatLevel::Literal>& steeringWheelHeatLevelSetting,
                           tarmac::timeprovider::TimerManagerInterface const& timeProvider);

    void request(FirstRowGen::HeatLevel const level);

  private:
    bool activationCheckOk() const;
    bool signalsOk() const;

    void handleVehicleMode();

    void setSteeringWheelHeat(autosar::SteerWhlHeatgOnCmdTyp level);

    autosar::SteerWhlHeatgOnCmdTyp convertHeatLevelToAutosar(FirstRowGen::HeatLevel level);
    FirstRowGen::HeatLevel const convertLevelTypeToHeatLevel(UserSelectionGen::LevelType const level);
    bool steeringWheelHeatersPresent();
    auto getTimeoutFromLocalConfig();

    NotifiableProperty<FirstRowGen::HeatAttribute>& shareHeatAttribute_;
    SubscriptionHandle shareHeatAttributeHandel_;
    SettingsProxyInterface<FirstRowGen::HeatLevel::Literal>& setting_;
    std::atomic_bool active_;
    std::chrono::steady_clock::time_point initialTime_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal_;
    ApplicationDataElement::DEReceiver<autosar::SteerWhlHeatgStrtAutCdnOk_info> steeringWheelHeatgAutCdnSignal_;

    ApplicationDataElement::DESender<autosar::SteerWhlHeatgOnReq_info> steeringWheelHeatSignal_;

    tarmac::timeprovider::TimerManagerInterface const& timeProvider_;
    std::chrono::milliseconds timeProviderTimeout_;

    std::recursive_mutex mutex_;

    ReadOnlyNotifiableProperty<UserSelectionGen::OffOnSelection>& autoSteeringWheelHeatOn_;
    ReadOnlyNotifiableProperty<UserSelectionGen::LevelSelection>& autoSteeringWheelHeatLevel_;

    void handleSteeringWheelHeatAutoSignal(void);
    void handleSteeringWheelHeatAutoMode();
};
