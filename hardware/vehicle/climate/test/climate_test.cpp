/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "ECD_dataelement.h"
#include "common_factory.h"
#include "first_row_factory.h"

// TODO(climateport): Parking and second row removed
//#include "parking_climate_factory.h"
//#include "second_and_third_row_factory.h"
#include "IDispatcher.h"
#include "climate_main.h"
#include "libsettings/settingsmanager.h"
#include "libsettings/settingsmanagerhidl.h"
#include "setting_factory.h"
#include "signal_proxy.h"
#include "time_provider.h"
#include "user_selection_factory.h"

#undef LOG_TAG
#define LOG_TAG "ClimateTest"

using namespace v0::org::volvocars::climate;
using namespace autosar;
using namespace std::chrono_literals;

int main() {
    /*
    std::cout << "hej";
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher = tarmac::eventloop::IDispatcher::CreateDispatcher();

    std::unique_ptr<ClimateMain> m;
    std::unique_ptr<v0::org::volvocars::climate::FirstRowStubImpl> climateService;
    dispatcher->EnqueueTask([&]() {
        m.reset(new ClimateMain(dispatcher));

        climateService.reset(new v0::org::volvocars::climate::FirstRowStubImpl(m->first_row, m->commonFactory_));
        climateService->initStubAdapter();
    });

    double t = 18;
    size_t levelIndex = 0;
    std::vector<FirstRow::HeatLevel> levels{
            {FirstRow::HeatLevel::OFF, FirstRow::HeatLevel::LO, FirstRow::HeatLevel::MED, FirstRow::HeatLevel::HI}};

    dispatcher->EnqueueTaskWithDelay(
            std::chrono::milliseconds(2000),
            [&]() {
                climateService->requestDriverTemperature(
                        t, [](CommonTypes::ReturnCode ret) { std::cout << "Return code: " << ret << std::endl; });
                climateService->requestSeatHeatDriver(levels.at(levelIndex), [](CommonTypes::ReturnCode ret) {
                    std::cout << "Return code: " << ret << std::endl;
                });

                levelIndex = (levelIndex + 1) % levels.size();
                t += 0.5;
                if (t > 24) {
                    t = 18;
                }
            },
            true);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    */
}

/*
class ClimateTest : public ::testing::Test {
  public:
    ClimateTest() :
      tarmacDispatcher_{tarmac::eventloop::IDispatcher::CreateDispatcher()},
      settingsManager_{new SettingsFramework::SettingsManagerHidl(*tarmacDispatcher_)},
      timeProvider_{tarmacDispatcher_},
      signalProxies_{},
      user_selection{settingsManager_},
      first_row{settingsManager_, timeProvider_, signalProxies_, user_selection, commonFactory_}
    {
    }

    std::shared_ptr<tarmac::eventloop::IDispatcher> tarmacDispatcher_;
    android::sp<SettingsFramework::SettingsManager> settingsManager_;
    tarmac::timeprovider::TimeProvider timeProvider_;

    signal_proxy::Proxies signalProxies_;
    common::daemon::Factory commonFactory_;
    UserSelectionFactory user_selection;
    FirstRowFactory first_row;


    ECDDataElement::DEInjector<ProfPenSts1_info> prof_pen_sts1_receiver_injector_;
    ECDDataElement::DEInjector<KeyReadStsToProfCtrl_info> key_read_sts_injector_;
    ECDDataElement::DEInjector<NrOfKeyAvl_info> nr_of_key_injector_;

    ECDDataElement::DESink<ProfChg_info> prof_chg_sink_;
    ECDDataElement::DESink<KeyReadReqFromSetgMgr_info> key_read_req_sink_;
};

TEST_F(ClimateTest, setFanLevel) {
    v0::org::volvocars::climate::FirstRowStubImpl climateService(first_row, commonFactory_);
    climateService.initStubAdapter();
    climateService.requestFanLevelFront(FirstRow::FanLevelFrontRequest::LVL5, [](CommonTypes::ReturnCode ret) {
        std::cout << "Return code: " << ret;
    });
}
*/