/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libsettings/setting.h>
#include <chrono>
#include <thread>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include "air_conditioner_logic.h"
#include "auto_climate_logic.h"
#include "fan_level_front_logic.h"
#include "ivi-logging.h"
#include "max_defroster_logic.h"
#include "mock_settingsmanager.h"
#include "notifiable_property.h"

namespace TestAirConditioner {

using namespace ApplicationDataElement;
using namespace autosar;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using namespace SettingsFramework;
using namespace ECDDataElement;

class AirConditionerTest : public ::testing::Test {
  public:
    NotifiableProperty<FirstRowGen::AirConditionerState> shareAirConditioner;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> shareMaxDefrost;
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> shareFanLevel;
    NotifiableProperty<AutoClimateLogic::AutoClimateEvent> shareAutoClimateEvent;
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> shareClimateReset;

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    CompatSetting<FirstRowGen::AirConditionerState::Literal, UserScope::USER> settingAirConditioner;
    CompatSetting<FirstRowGen::AirConditionerState::Literal, UserScope::NOT_USER_RELATED> settingAirConditionerDyno;
    SettingsProxy<FirstRowGen::AirConditionerState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>
            settingAirConditionerProxy;
    DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
    DEInjector<autosar::ClimaActv_info> ClimaActv_;

    // Signals (needed here so we can be sure they are instantiated before the test start)
    ApplicationDataElement::DESender<autosar::HmiCmptmtCoolgReq_info> hmiCmptmtCoolgReq_;

    /*==============================================*/
    /*               Help functions                 */
    /*==============================================*/

    AirConditionerTest()
        : shareAirConditioner{FirstRowGen::AirConditionerState::DISABLED},
          shareMaxDefrost{FirstRowGen::MaxDefrosterState::OFF},
          shareFanLevel{FirstRowGen::FanLevelFrontValue::OFF},
          shareAutoClimateEvent{AutoClimateLogic::AutoClimateEvent::NO_REQ},
          shareClimateReset{ClimateResetLogic::ClimateResetEvent::NO_REQ},
          settingAirConditioner(SettingId::TestSetting1, FirstRowGen::AirConditionerState::OFF, settingsManager),
          settingAirConditionerDyno(SettingId::TestSetting2, FirstRowGen::AirConditionerState::OFF, settingsManager),
          settingAirConditionerProxy(settingAirConditioner, settingAirConditionerDyno) {}

    void SetUp() {
        setClimaActv(OnOff1::Off);
        setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);
        setFanLevelSetting(FirstRowGen::FanLevelFrontValue::OFF);
        setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent::NO_REQ);
        setClimateReset(ClimateResetLogic::ClimateResetEvent::NO_REQ);
        settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
        resetVehicleMode();
    }

    void TearDown() {
        setClimaActv(OnOff1::Off);
        setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);
        setFanLevelSetting(FirstRowGen::FanLevelFrontValue::OFF);
        setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent::NO_REQ);
        setClimateReset(ClimateResetLogic::ClimateResetEvent::NO_REQ);
        settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
        resetVehicleMode();
    }

    void setVehicleMode(UsgModSts1 usgModeSts1, CarModSts1 carModSts1) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        drvMode.CarModSts1_ = carModSts1;
        driverMode.inject(drvMode);
    }

    void resetVehicleMode() {
        VehModMngtGlbSafe1 drvMode;
        drvMode.CarModSts1_ = CarModSts1::CarModFcy;
        drvMode.UsgModSts = UsgModSts1::UsgModAbdnd;
        drvMode.CarModSubtypWdCarModSubtyp = 0;
        drvMode.EgyLvlElecMai = 0;
        drvMode.EgyLvlElecSubtyp = 0;
        drvMode.PwrLvlElecMai = 0;
        drvMode.PwrLvlElecSubtyp = 0;
        drvMode.FltEgyCnsWdSts = FltEgyCns1::NoFlt;
        drvMode.Chks = 0;
        drvMode.Cntr = 0;
        driverMode.inject(drvMode);
    }

    void setVehicleModeError() { driverMode.error(12); }

    void setClimaActv(OnOff1 onoff) {
        DEInjector<ClimaActv_info> climaActv;
        climaActv.inject(onoff);
    }

    void setFanLevelSetting(FirstRowGen::FanLevelFrontValue level) {
        // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
        shareFanLevel.set(level);
    }

    //  MaxDefrostValue {OFF, ON};
    void setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState frost) { shareMaxDefrost.set(frost); }

    //  AutoClimate {OFF, ON};
    void setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent val) { shareAutoClimateEvent.set(val); }

    void setClimateReset(ClimateResetLogic::ClimateResetEvent event) { shareClimateReset.set(event); }
};

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(AirConditionerTest, test_default_state_of_ac_object) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    EXPECT_EQ(FirstRowGen::AirConditionerState::DISABLED, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_state_is_not_available_abandoned_usg_mode) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_ac_state_is_ok_if_in_usg_mode_driving) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_ac_state_is_temp_not_avail_if_in_usg_mode_is_active) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModActv, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_ac_state_is_temp_not_avail_if_in_usg_mode_is_abandoned) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_ac_state_is_temp_not_avail_if_in_usg_mode_is_inactive) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModInActv, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_we_can_jump_between_ac_state_ok_and_not_avail_when_usage_mode_changes) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Set to driving
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // InActive
    setVehicleMode(UsgModSts1::UsgModInActv, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Back to driving
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Test convinienc
    setVehicleMode(UsgModSts1::UsgModCnvinc, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Back to driving
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_ac_state_only_is_ok_in_car_mode_norma_or_dyno) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    // Set to driving and Normal car mode
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Set to driving and Dyno car mode
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Set to driving and Crash mode
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Set to driving and Transport mode
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModTrnsp);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Set to driving and Fcy mode ???
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    // Back to normal car mode
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_ac_state_is_correct_if_parking_climate_is_on_or_off) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModActv, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::Off);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    setClimaActv(OnOff1::On);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);

    setClimaActv(OnOff1::Off);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::DISABLED);
}

TEST_F(AirConditionerTest, test_that_fan_level_off_sets_ac_state_to_tmp_not_avail_and_back) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::AirConditionerState::DISABLED, shareAirConditioner.get());

    // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());

    // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::OFF);

    EXPECT_EQ(FirstRowGen::AirConditionerState::DISABLED, shareAirConditioner.get());

    // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_turns_off_if_saved_setting_is_off) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);
}

TEST_F(AirConditionerTest, test_that_ac_turns_auto_if_saved_setting_is_auto_and_car_mode_is_normal) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_ac_turns_off_if_saved_setting_is_auto_but_carmode_is_dyno) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());
}

TEST_F(AirConditionerTest,
       test_that_ac_turns_off_if_saved_setting_is_auto_but_carmode_is_dyno_but_we_still_can_turn_ac_off_and_auto) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::OFF);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);
}

TEST_F(AirConditionerTest, test_that_ac_toggels_between_off_and_auto) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);

    // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::OFF);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_ac_sends_signal_to_turn_ac_off_or_auto) {
    // Listen to outgoing dataelement
    DESink<autosar::HmiCmptmtCoolgReq_info> hmiCmptmtCoolgReq;

    HmiCmptmtCoolgReq outvalue = HmiCmptmtCoolgReq::Off;
    hmiCmptmtCoolgReq.subscribe([&hmiCmptmtCoolgReq, &outvalue]() { outvalue = hmiCmptmtCoolgReq.get().value(); });

    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    // Set AC to Auto
    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);
    EXPECT_EQ(HmiCmptmtCoolgReq::Auto, outvalue);

    // Set AC to Off
    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::OFF);
    EXPECT_EQ(HmiCmptmtCoolgReq::Off, outvalue);

    // Set AC to Auto
    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);
    EXPECT_EQ(HmiCmptmtCoolgReq::Auto, outvalue);
}

TEST_F(AirConditionerTest, test_that_if_a_request_for_disable_state_doesnt_set_ac_in_disable_state) {
    DESink<autosar::HmiCmptmtCoolgReq_info> hmiCmptmtCoolgReq;

    // Listen to outgoing dataelement
    HmiCmptmtCoolgReq outvalue = HmiCmptmtCoolgReq::Off;
    hmiCmptmtCoolgReq.subscribe([&hmiCmptmtCoolgReq, &outvalue]() { outvalue = hmiCmptmtCoolgReq.get().value(); });

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);

    // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    // Set AC to Auto
    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);
    EXPECT_EQ(HmiCmptmtCoolgReq::Auto, outvalue);

    // Set AC to Disabled !!!
    ac.requestAirConditioner(
            static_cast<FirstRowGen::AirConditionerRequest::Literal>(FirstRowGen::AirConditionerState::DISABLED));
    EXPECT_EQ(HmiCmptmtCoolgReq::Auto, outvalue);  // Retain previous value
}

TEST_F(AirConditionerTest, test_that_when_auto_climate_is_activated_ac_turns_to_auto) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::OFF);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    // Auto climate test
    shareAutoClimateEvent.set(AutoClimateLogic::AutoClimateEvent::ACTIVATED);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_when_auto_climate_is_activated_and_then_deactivated_ac_stays_in_auto) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);

    shareAutoClimateEvent.set(AutoClimateLogic::AutoClimateEvent::ACTIVATED);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);

    shareAutoClimateEvent.set(AutoClimateLogic::AutoClimateEvent::NO_REQ);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_when_climate_reset_is_activated_ac_turns_to_auto) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::OFF);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    setClimateReset(ClimateResetLogic::ClimateResetEvent::ACTIVATED);
    setClimateReset(ClimateResetLogic::ClimateResetEvent::NO_REQ);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_ac_enters_system_error_when_signal_values_indicate_error) {
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setVehicleModeError();

    EXPECT_EQ(FirstRowGen::AirConditionerState::SYSTEM_ERROR, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_turns_to_auto_when_max_defrost_is_turned_on) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_ac_stays_in_auto_when_max_defrost_is_turned_on) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);
}

TEST_F(AirConditionerTest, test_that_ac_goes_back_to_its_saved_value_when_max_defrost_is_turned_on_then_off) {
    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);
    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::AUTO);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);

    EXPECT_TRUE(shareAirConditioner.get() == FirstRowGen::AirConditionerState::OFF);
}

TEST_F(AirConditionerTest, test_that_ac_can_not_be_turned_on_and_off_even_if_fan_is_not_available) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::DISABLED);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::AirConditionerState::DISABLED, shareAirConditioner.get());

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::OFF);
    EXPECT_EQ(FirstRowGen::AirConditionerState::DISABLED, shareAirConditioner.get());

    ac.requestAirConditioner(FirstRowGen::AirConditionerRequest::AUTO);
    EXPECT_EQ(FirstRowGen::AirConditionerState::DISABLED, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_setting_is_saved_when_user_presses_auto_climate) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());

    // Auto climate activated
    setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent::ACTIVATED);
    setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent::NO_REQ);

    EXPECT_EQ(FirstRowGen::AirConditionerState::AUTO, shareAirConditioner.get());

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL2);

    // AC stay in Auto
    EXPECT_EQ(FirstRowGen::AirConditionerState::AUTO, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_is_initilized_to_the_ac_setting_for_dyno_off_when_reciving_carmode) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_is_initilized_to_the_ac_setting_for_dyno_off_when_carmode_is_set) {
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_is_initilized_to_the_ac_setting_for_dyno_auto) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());
}

TEST_F(AirConditionerTest, test_that_ac_is_work_with_profile_change) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    settingAirConditioner.set(FirstRowGen::AirConditionerState::OFF);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirConditionerLogic ac(settingAirConditionerProxy,
                           shareAirConditioner,
                           shareFanLevel,
                           shareMaxDefrost,
                           shareAutoClimateEvent,
                           shareClimateReset);

    EXPECT_EQ(FirstRowGen::AirConditionerState::OFF, shareAirConditioner.get());

    // Simulate profile change by setting settings value.
    settingAirConditioner.set(FirstRowGen::AirConditionerState::AUTO);

    EXPECT_EQ(FirstRowGen::AirConditionerState::AUTO, shareAirConditioner.get());
}
//==================================================================================
}  // namespace TestAirConditioner
