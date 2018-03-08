/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "auto_climate_logic.h"
#include "climate_reset_logic.h"
#include "fan_level_front_logic.h"
#include "legacydispatcher.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"
#include "settings_proxy.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"
#include "v0/org/volvocars/climate/UserSelection.hpp"

#include <Application_dataelement.h>
#include <libsettings/setting.h>
#include <memory>

using namespace SettingsFramework;

class ManualRecircLogic {
    using FirstRowGen = v0::org::volvocars::climate::FirstRow;
    using userSelectionGen = v0::org::volvocars::climate::UserSelection;

  public:
    ManualRecircLogic(const vcc::LocalConfigReaderInterface* lcfg,
                      ReadOnlyNotifiableProperty<userSelectionGen::OffOnSelection>& RecircTimer,
                      ReadOnlyNotifiableProperty<userSelectionGen::OffOnSelection>& AirQualitySensor,
                      SettingsProxy<FirstRowGen::ManualRecircState::Literal, UserScope::USER,
                                    UserScope::NOT_USER_RELATED>& settingManualRecirc,
                      NotifiableProperty<FirstRowGen::ManualRecircState>& sharedManualRecirc,
                      ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset,
                      ILegacyDispatcher& timerDispatcher,
                      ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
                      ReadOnlyNotifiableProperty<AutoClimateLogic::AutoClimateEvent>& autoClimate,
                      ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster);

    void requestManualRecirc(FirstRowGen::ManualRecircRequest value);

  private:
    void setStateAndValue(bool isActive, FirstRowGen::FanLevelFrontValue fanLevel,
                          FirstRowGen::MaxDefrosterState maxDefroster);
    void sendSignal();
    bool signalsOk();
    bool activationCheck(const autosar::UsgModSts1 usgMode, const autosar::CarModSts1 carMode,
                         const autosar::OnOff1 parkingClimate);

    ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& shareClimateReset;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& shareMaxDefroster;
    ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& shareFanLevelFront;
    ReadOnlyNotifiableProperty<AutoClimateLogic::AutoClimateEvent>& shareAutoClimate;
    ReadOnlyNotifiableProperty<userSelectionGen::OffOnSelection>& shareRecircTimer;
    ReadOnlyNotifiableProperty<userSelectionGen::OffOnSelection>& shareAirQualitySensor;

    SubscriptionHandle shareMaxDefrosterId;
    SubscriptionHandle shareFanLevelFrontId;
    SubscriptionHandle shareAutoClimateId;
    SubscriptionHandle shareClimateResetId;
    SubscriptionHandle shareAirQualitySensorId;

    SettingsProxy<FirstRowGen::ManualRecircState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>&
            sManualRecircProxy;
    FirstRowGen::ManualRecircState::Literal sManualRecircProxyGETPORT;

    NotifiableProperty<FirstRowGen::ManualRecircState>& sharedManualRecirc_;
    std::recursive_mutex mutex_;
    bool isActive_;
    bool carConfig_;
    ILegacyDispatcher& timerDispatcher_;
    std::chrono::minutes timeout_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModSts_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActiveSignal_;
    ApplicationDataElement::DESender<autosar::HmiHvacRecircCmd_info> hmiHvacRecircCmd_;
};
