/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "notifiable_property.h"
#include "settings_proxy.h"
#include "timer_manager_interface.h"

#include <atomic>
#include <memory>
#include <mutex>

class SeatHeatLogic {
  public:
    enum class UserLocation { DRIVER, PASSENGER };

  private:
    using FirstRowGen = v0::org::volvocars::climate::FirstRow;
    using UserSelectionGen = v0::org::volvocars::climate::UserSelection;

  public:
    SeatHeatLogic(UserLocation userLocation, autosar::HmiSeatClima& seatClimate,
                  NotifiableProperty<FirstRowGen::HeatAttribute>& heatAttribute,
                  NotifiableProperty<UserSelectionGen::OffOnSelection>& autoSeatHeatOn,
                  NotifiableProperty<UserSelectionGen::LevelSelection>& autoSeatHeatLevel,
                  SettingsProxyInterface<FirstRowGen::HeatLevel::Literal>& seatHeatLevelSetting,
                  tarmac::timeprovider::TimerManagerInterface const& timeProvider);

    ~SeatHeatLogic();

    void request(FirstRowGen::HeatLevel const level);

  private:
    autosar::SeatClimaLvl& getSeatHeatSignal();

    bool activationCheck() const;
    bool signalsOk() const;
    bool inDynoMode() const;
    void handleSeatHeatAutoSignal();
    void handleAuto();
    void handleVehicleMode();
    void sendSignal(autosar::SeatClimaLvl level);

    UserLocation userLocation_;

    NotifiableProperty<FirstRowGen::HeatAttribute>& shareHeatAttribute_;
    SubscriptionHandle shareHeatAttributeHandel_;
    SettingsProxyInterface<FirstRowGen::HeatLevel::Literal>& sHeatLevel_;
    std::atomic_bool active_;
    std::chrono::steady_clock::time_point initialTime_;

    autosar::HmiSeatClima& seatClimate_;
    autosar::SeatClimaLvl& seatHeatSignal_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal_;
    ApplicationDataElement::DEReceiver<autosar::SeatHeatgAutCdn_info> seatHeatgAutCdnSignal_;
    ApplicationDataElement::DESender<autosar::HmiSeatClima_info> seatClimateSignal_;

    tarmac::timeprovider::TimerManagerInterface const& timeProvider_;
    std::chrono::milliseconds timeProviderTimeout_;

    std::recursive_mutex mutex_;

    NotifiableProperty<UserSelectionGen::OffOnSelection>& autoSeatHeatOn_;
    NotifiableProperty<UserSelectionGen::LevelSelection>& autoSeatHeatLevel_;
};
