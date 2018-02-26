/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include "common_factory.h"
#include "first_row_factory.h"

//TODO(climateport): Parking and second row removed
//#include "parking_climate_factory.h"
//#include "second_and_third_row_factory.h"
#include "setting_factory.h"
#include "signal_proxy.h"
#include "user_selection_factory.h"
#include "libsettings/settingsmanager.h"
#include "IDispatcher.h"
#include "time_provider.h"

class ClimateMain final
{
public:
    ClimateMain();

private:
    std::shared_ptr<tarmac::eventloop::IDispatcher> tarmacDispatcher_;
    android::sp<SettingsFramework::SettingsManager> settingsManager_;
    tarmac::timeprovider::TimeProvider timeProvider_;
    
    signal_proxy::Proxies         signalProxies_;
    common::daemon::Factory       commonFactory_;
    UserSelectionFactory          user_selection;
    FirstRowFactory               first_row;

    //TODO(climateport): Parking and second row removed
    //ParkingClimateFactory         parking_climate;
    //second_and_third_row::Factory second_and_third_row;
};
