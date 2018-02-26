/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "notifiable_property.h"
#include "settings_proxy.h"
#include "v0/org/volvocars/climate/CommonTypes.hpp"
#include "v0/org/volvocars/climate/FirstRow.hpp"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>

using FirstRowGen    = v0::org::volvocars::climate::FirstRow;
using CommonTypesGen = v0::org::volvocars::climate::CommonTypes;

class TemperatureSyncLogic
{
public:
    TemperatureSyncLogic(NotifiableProperty<FirstRowGen::TemperatureSyncState>&      temperatureSync,
                         ReadOnlyNotifiableProperty<double>&                         driverTemp,
                         ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      driverTempHiLoN,
                         ReadOnlyNotifiableProperty<double>&                         passengerTemp,
                         ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      passengerTempHiLoN,
                         ReadOnlyNotifiableProperty<double>&                         rearLeftTemp,
                         ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      rearLeftTempHiLoN,
                         ReadOnlyNotifiableProperty<double>&                         rearRightTemp,
                         ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      rearRightTempHiLoN,
                         ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
                         std::unique_ptr<SettingsProxy<bool, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER>>                        temperatureSyncSetting);

    ~TemperatureSyncLogic() = default;

    CommonTypesGen::ReturnCode request(FirstRowGen::TemperatureSyncRequest temperatureSyncRequest);

private:
    bool isActive();
    void updateState();

    NotifiableProperty<FirstRowGen::TemperatureSyncState>&      temperatureSync_;
    ReadOnlyNotifiableProperty<double>&                         driverTemp_;
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      driverTempHiLoN_;
    ReadOnlyNotifiableProperty<double>&                         passengerTemp_;
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      passengerTempHiLoN_;
    ReadOnlyNotifiableProperty<double>&                         rearLeftTemp_;
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      rearLeftTempHiLoN_;
    ReadOnlyNotifiableProperty<double>&                         rearRightTemp_;
    ReadOnlyNotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      rearRightTempHiLoN_;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster_;

    std::unique_ptr<SettingsProxy<bool, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER>> temperatureSyncSetting_;

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info>          climaActvSignal_;

    bool carConfigOk_;

    std::recursive_mutex mutex_;

    std::vector<SubscriptionHandle> subscriptions_;
};
