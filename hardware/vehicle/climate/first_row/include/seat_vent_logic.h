/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "timer_manager_interface.h"
#include "notifiable_property.h"
#include "settings_proxy.h"
#include <cc_parameterlist.h>
#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

#include <atomic>
#include <memory>
#include <mutex>

class SeatVentLogic
{
public:
    enum class UserLocation
    {
        DRIVER,
        PASSENGER
    };

private:
    using FirstRowGen = v0::org::volvocars::climate::FirstRow;

public:
    SeatVentLogic(UserLocation                                    userLocation,
                  autosar::HmiSeatClima&                          seatClimate,
                  NotifiableProperty<FirstRowGen::VentAttribute>& ventAttribute,
                  SettingsProxyInterface<FirstRowGen::VentLevel::Literal>& seatVentLevelSetting);

public:
    ~SeatVentLogic();

    void request(FirstRowGen::VentLevel const level);

private:
    autosar::SeatClimaLvl& getSeatVentSignal();

    bool activationCheckOk() const;
    void handleVehicleMode();
    bool seatVentilationPresent();
    void sendSignal(autosar::SeatClimaLvl level);

    autosar::SeatClimaLvl convertVentLevelToAutosar(FirstRowGen::VentLevel level);

    UserLocation           userLocation_;

    NotifiableProperty<FirstRowGen::VentAttribute>& shareVentAttribute_;
    SubscriptionHandle                              shareVentAttributeHandel_;
    SettingsProxyInterface<FirstRowGen::VentLevel::Literal>& sVentLevel_;

    autosar::HmiSeatClima& seatClimate_;
    autosar::SeatClimaLvl& seatVentSignal_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal_;
    ApplicationDataElement::DESender<autosar::HmiSeatClima_info>         seatClimateSignal_;

    std::recursive_mutex mutex_;
};
