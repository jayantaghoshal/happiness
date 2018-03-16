/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <Application_dataelement.h>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include "auto_climate_logic.h"
#include "climate_reset_logic.h"
#include "fan_level_front_logic.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"
#include "settings_proxy.h"

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using namespace SettingsFramework;

class AirConditionerLogic {
  public:
    explicit AirConditionerLogic(
            SettingsProxy<FirstRowGen::AirConditionerState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>&
                    sAirConditioner,
            NotifiableProperty<FirstRowGen::AirConditionerState>& airConditioner,
            ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
            ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
            ReadOnlyNotifiableProperty<AutoClimateLogic::AutoClimateEvent>& autoClimateEvent,
            ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset);
    ~AirConditionerLogic();

    void requestAirConditioner(FirstRowGen::AirConditionerRequest value);

  private:
    SettingsProxy<FirstRowGen::AirConditionerState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>&
            settingAirConditioner;
    FirstRowGen::AirConditionerState::Literal settingAirConditionerGETPORT;

    SubscriptionHandle airConditionerId_;
    SubscriptionHandle fanLevelFrontId_;
    SubscriptionHandle maxDefrosterId_;
    SubscriptionHandle autoClimateEventId_;
    SubscriptionHandle climateResetId_;

    NotifiableProperty<FirstRowGen::AirConditionerState>& shareAirConditioner;
    ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& shareFanLevelFront;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& shareMaxDefroster;
    ReadOnlyNotifiableProperty<AutoClimateLogic::AutoClimateEvent>& shareAutoClimateEvent;
    ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& shareClimateReset;

    autosar::OnOff1 climateActive_;
    bool inCarModeDyno_;
    int CarModeCallbackId_;
    int UsgModeCallbackId_;
    std::mutex mutex_;
    bool isActive_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActiveSignal_;
    ApplicationDataElement::DESender<autosar::HmiCmptmtCoolgReq_info> hmiCmptmtCoolgReq_;

    void setState(bool isActive,
                  FirstRowGen::FanLevelFrontValue fanLevel,
                  FirstRowGen::AirConditionerState airConditioner,
                  bool isInError = false);
    void sendAcSignal();
    bool signalsOk() const;
    bool activationCheck(const autosar::UsgModSts1 usgMode,
                         const autosar::CarModSts1 carMode,
                         const autosar::OnOff1 parkingClimate) const;
    bool inCarModeDyno() const;
};
