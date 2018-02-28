/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "common_factory.h"
#include "first_row_factory.h"

// TODO(climateport): Parking and second row removed
//#include "parking_climate_factory.h"
//#include "second_and_third_row_factory.h"
#include "IDispatcher.h"
#include "libsettings/settingsmanager.h"
#include "setting_factory.h"
#include "signal_proxy.h"
#include "time_provider.h"
#include "user_selection_factory.h"

class ClimateMain final {
  public:
    ClimateMain(const std::shared_ptr<tarmac::eventloop::IDispatcher>& tarmacDispatcher);

    // TODO:(climateport): temporary public to make testing easier, should be private
  public:
    std::shared_ptr<tarmac::eventloop::IDispatcher> tarmacDispatcher_;
    android::sp<SettingsFramework::SettingsManager> settingsManager_;
    tarmac::timeprovider::TimeProvider timeProvider_;
    bool hackToInitGlobalLegacyDispatcher;
    signal_proxy::Proxies signalProxies_;
    common::daemon::Factory commonFactory_;
    UserSelectionFactory user_selection;
    FirstRowFactory first_row;

    // TODO(climateport): Parking and second row removed
    // ParkingClimateFactory         parking_climate;
    // second_and_third_row::Factory second_and_third_row;
};
