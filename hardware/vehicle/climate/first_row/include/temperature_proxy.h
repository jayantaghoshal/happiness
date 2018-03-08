/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "notifiable_property.h"
#include "settings_proxy.h"

#include <Application_dataelement.h>
#include <libsettings/setting.h>
#include <mutex>
#include <utility>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <vector>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class TemperatureProxy {
  public:
    TemperatureProxy(NotifiableProperty<double>& storedDriverTemp,
                     NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedDriverTempHiLoN,
                     NotifiableProperty<double>& storedPassengerTemp,
                     NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedPassengerTempHiLoN,
                     NotifiableProperty<double>& driverTemp,
                     NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& driverTempHiLoN,
                     NotifiableProperty<double>& passengerTemp,
                     NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& passengerTempHiLoN,
                     NotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
                     SettingsProxy<double, SettingsFramework::UserScope::USER,
                                   SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperatureDriverProxy,
                     SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::USER,
                                   SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperatureDriverHiLoNProxy,
                     SettingsProxy<double, SettingsFramework::UserScope::NOT_USER_RELATED,
                                   SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperaturePassengerProxy,
                     SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED,
                                   SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperaturePassengerHiLoNProxy,
                     autosar::HmiCmptmtTSp& hmiCmptmtTSp);

    ~TemperatureProxy();

  private:
    bool isCarModDyno() const;

    template <class Setting, class SharedObject, typename T>
    void connect(Setting& setting, SharedObject& sharedObject, T& member);
    template <typename Value, class Setting>
    void update(Value value, Value& signalValue, Setting& setting);

    void updateDriver(double temp, double& signalTemp);
    void updateDriver(autosar::HmiCmptmtTSpSpcl tempHiLoN, autosar::HmiCmptmtTSpSpcl& signalTempHiLoN);
    void updatePassenger(double temp, double& signalTemp);
    void updatePassenger(autosar::HmiCmptmtTSpSpcl tempHiLoN, autosar::HmiCmptmtTSpSpcl& signalTempHiLoN);

    void handleCarMod(bool isPassengerPresent);

    NotifiableProperty<double>& storedDriverTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedDriverTempHiLoN_;
    NotifiableProperty<double>& storedPassengerTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedPassengerTempHiLoN_;
    NotifiableProperty<double>& driverTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& driverTempHiLoN_;
    NotifiableProperty<double>& passengerTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& passengerTempHiLoN_;
    NotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster_;

    SettingsProxy<double, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>&
            sTemperatureDriverProxy_;
    SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::USER,
                  SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperatureDriverHiLoNProxy_;
    SettingsProxy<double, SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperaturePassengerProxy_;
    SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>& sTemperaturePassengerHiLoNProxy_;
    double sTemperatureDriverProxyGETPORT_;
    autosar::HmiCmptmtTSpSpcl sTemperatureDriverHiLoNProxyGETPORT_;
    double sTemperaturePassengerProxyGETPORT_;
    autosar::HmiCmptmtTSpSpcl sTemperaturePassengerHiLoNProxyGETPORT_;

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;
    ApplicationDataElement::DESender<autosar::HmiCmptmtTSp_info> hmiCmptmtTSp_;

    autosar::HmiCmptmtTSp& currentHmiCmptmtTSp_;

    std::vector<SubscriptionHandle> subscriptions_;
    std::mutex updateLock_;
};
