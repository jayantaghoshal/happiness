/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "air_distribution_logic.h"

#include "auto_climate_logic.h"
#include "carconfig_mock.h"
#include "climate_reset_logic.h"
#include "enum_helper.h"
#include "fan_level_front_logic.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"
#include "settings_proxy.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gtest/gtest.h>
#include <tuple>
#include "mock_settingsmanager.h"
using FirstRowGen = v0::org::volvocars::climate::FirstRow;

using namespace testing;

namespace TestAirDistribution {
using namespace autosar;
using CC175 = CarConfigParams::CC175_HvacVariantsType;
using AirDistributionAngle = FirstRowGen::AirDistributionAngle;
using AirDistributionAngleRequest = FirstRowGen::AirDistributionAngleRequest;
using AutoClimateState = FirstRowGen::AutoClimateState;

auto const sAirDistributionSettingName = std::string{"AirDistributionTest.sAirDistribution"};

class AirDistributionTest : public Test {
  public:
    AirDistributionTest() : carConfig_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC175::HVAC_Small_1_zone)));

        DataElementFramework::instance().reset();
        setClimaActv(OnOff1::Off);
        resetVehicleMode();
    }

  protected:
    void setVehicleMode(autosar::UsgModSts1 usgModeSts1, autosar::CarModSts1 carModSts1) {
        autosar::VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        drvMode.CarModSts1_ = carModSts1;
        driverMode.inject(drvMode);
    }

    void setUsgMode(autosar::UsgModSts1 usgModeSts1) {
        autosar::VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        driverMode.inject(drvMode);
    }

    void resetVehicleMode() {
        autosar::VehModMngtGlbSafe1 drvMode;
        drvMode.CarModSts1_ = autosar::CarModSts1::CarModFcy;
        drvMode.UsgModSts = autosar::UsgModSts1::UsgModAbdnd;
        drvMode.CarModSubtypWdCarModSubtyp = 0;
        drvMode.EgyLvlElecMai = 0;
        drvMode.EgyLvlElecSubtyp = 0;
        drvMode.PwrLvlElecMai = 0;
        drvMode.PwrLvlElecSubtyp = 0;
        drvMode.FltEgyCnsWdSts = autosar::FltEgyCns1::NoFlt;
        drvMode.Chks = 0;
        drvMode.Cntr = 0;
        driverMode.inject(drvMode);
    }

    void setFanLevelSetting(FirstRowGen::FanLevelFrontValue level) {
        // Inject fan level values, FanLevelFrontValue {OFF, LVL1, LVL2, LVL3, LVL4, LVL5, MAX};
        shareFanLevel.set(level);
    }

    void setAutoClimateState(AutoClimateState value) { shareAutoClimate.set(value); }

    void setMaxDefrosterValue(FirstRowGen::MaxDefrosterState value) { shareMaxDefroster.set(value); }

    void setClimaActv(OnOff1 onoff) {
        ECDDataElement::DEInjector<ClimaActv_info> climaActv;
        climaActv.inject(onoff);
    }
    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    NiceMock<CarConfigMock> carConfig_;

    NotifiableProperty<AirDistributionAngle> airDistributionAngle_{AirDistributionAngle::DISABLED};

    CompatSetting<FirstRowGen::AirDistributionAngle::Literal, SettingsFramework::UserScope::USER> sAirDistribution{
            SettingId::TestSetting1, AirDistributionAngle::FLOOR, settingsManager};

    CompatSetting<FirstRowGen::AirDistributionAngle::Literal, SettingsFramework::UserScope::NOT_USER_RELATED>
            sAirDistributionDyno{SettingId::TestSetting1, AirDistributionAngle::AUTO, settingsManager};

    SettingsProxy<FirstRowGen::AirDistributionAngle::Literal, SettingsFramework::UserScope::USER,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            sAirDistributionProxy{sAirDistribution, sAirDistributionDyno};

    NotifiableProperty<FirstRowGen::FanLevelFrontValue> shareFanLevel{FirstRowGen::FanLevelFrontValue::DISABLED};
    NotifiableProperty<FirstRowGen::MaxDefrosterState> shareMaxDefroster{FirstRowGen::MaxDefrosterState::DISABLED};
    NotifiableProperty<AutoClimateState> shareAutoClimate{AutoClimateState::DISABLED};
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> shareClimateResetEvent{
            ClimateResetLogic::ClimateResetEvent::NO_REQ};

    ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
};

/**
* GIVEN that CarMode is dyno and air distribution setting is not AUTO
* WHEN  AirDistributionLogic is instantiated
* THEN  the distribution angle should be set to AUTO
*/
TEST_F(AirDistributionTest, test_default_state_dyno) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());
}

/**
 * GIVEN that CarMode is dyno
 * WHEN  air distribution angle is changed
 * THEN  the distribution setting should not be updated
 */
TEST_F(AirDistributionTest, test_setting_not_saved_in_dyno_mode) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());

    airDistributionLogic.request(AirDistributionAngleRequest::FLOOR_VENT_DEFROST);

    EXPECT_EQ(AirDistributionAngle::FLOOR_VENT_DEFROST, airDistributionAngle_.get());
    EXPECT_EQ(AirDistributionAngle::FLOOR, static_cast<AirDistributionAngle::Literal>(sAirDistribution.get()));
}

/**
 * GIVEN that CarMode is normal and air distribution setting is FLOOR
 * WHEN  AirDistributionLogic is instantiated
 * THEN  the distribution angle should be set to FLOOR
 */
TEST_F(AirDistributionTest, test_default_state) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
}

/**
 * GIVEN that the HVAC type is Manual_AC
 * WHEN  AUTO is requested
 * THEN  the distribution angle shall retain its current value
 */
TEST_F(AirDistributionTest, auto_not_available_when_mcc) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillByDefault(Return(to_undrl(CC175::Manual_AC)));

    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    airDistributionLogic.request(AirDistributionAngleRequest::AUTO);
    EXPECT_NE(airDistributionAngle_.get(), AirDistributionAngle::AUTO);
    EXPECT_EQ(AirDistributionAngle::FLOOR, static_cast<AirDistributionAngle::Literal>(sAirDistribution.get()));
}

/**
 * GIVEN that the HVAC type is not Manual_AC
 * WHEN  a distribution angle is requested
 * THEN  the distribution angle shall be updated
 */
TEST_F(AirDistributionTest, test_setting_angles_ecc) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    for (auto angle : {AirDistributionAngleRequest::AUTO, AirDistributionAngleRequest::FLOOR,
                       AirDistributionAngleRequest::VENT, AirDistributionAngleRequest::DEFROST,
                       AirDistributionAngleRequest::FLOOR_VENT, AirDistributionAngleRequest::FLOOR_DEFROST,
                       AirDistributionAngleRequest::VENT_DEFROST, AirDistributionAngleRequest::FLOOR_VENT_DEFROST}) {
        airDistributionLogic.request(angle);
        EXPECT_EQ(airDistributionAngle_.get(), angle);
    }
}

/**
 * GIVEN that the HVAC type is Manual_AC
 * WHEN  a distribution angle requested is not AUTO
 * THEN  the distribution angle shall be updated
 */
TEST_F(AirDistributionTest, test_setting_angles_mcc) {
    ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillByDefault(Return(to_undrl(CC175::Manual_AC)));
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    for (auto angle :
         {AirDistributionAngleRequest::FLOOR, AirDistributionAngleRequest::VENT, AirDistributionAngleRequest::DEFROST,
          AirDistributionAngleRequest::FLOOR_VENT, AirDistributionAngleRequest::FLOOR_DEFROST,
          AirDistributionAngleRequest::VENT_DEFROST, AirDistributionAngleRequest::FLOOR_VENT_DEFROST}) {
        airDistributionLogic.request(angle);
        EXPECT_EQ(airDistributionAngle_.get(), angle);
    }
}

/**
 * GIVEN system is functioning normally AND Hvac type is not Manual_Ac
 * WHEN  fan level is OFF
 * THEN  air distribution shall be unavailable AND angle shall be AUTO
 */
TEST_F(AirDistributionTest, test_availability_when_fan_level_is_off) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());

    // Switch off fans
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::OFF);
    EXPECT_EQ(AirDistributionAngle::DISABLED, airDistributionAngle_.get());
}

/**
 * GIVEN system is functioning normally
 * WHEN  auto climate is enabled
 * THEN  air distribution angle shall be AUTO
 */
TEST_F(AirDistributionTest, test_autoclimate_on_enables_auto_angle) {
    autosar::HmiCmptmtAirDistbnFrnt outSignal(static_cast<autosar::HmiCmptmtAirDistbnFrnt>(-1));
    ECDDataElement::DESink<autosar::HmiCmptmtAirDistbnFrnt_info> AirDistributionSignal;
    AirDistributionSignal.subscribe(
            [&AirDistributionSignal, &outSignal]() { outSignal = AirDistributionSignal.get().value(); });

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::MAX);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    setAutoClimateState(AutoClimateState::ON);
    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());
    EXPECT_EQ(autosar::HmiCmptmtAirDistbnFrnt::Aut, outSignal);

    setAutoClimateState(AutoClimateState::OFF);
    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());
}

/**
 * GIVEN system is functioning normally
 * WHEN  auto climate is disabled
 * THEN  air distribution angle shall be restored
 */
TEST_F(AirDistributionTest, test_autoclimate_off_restores_previous_angle) {
    autosar::HmiCmptmtAirDistbnFrnt outSignal(static_cast<autosar::HmiCmptmtAirDistbnFrnt>(-1));
    ECDDataElement::DESink<autosar::HmiCmptmtAirDistbnFrnt_info> AirDistributionSignal;
    AirDistributionSignal.subscribe(
            [&AirDistributionSignal, &outSignal]() { outSignal = AirDistributionSignal.get().value(); });

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    airDistributionAngle_.set(AirDistributionAngle::AUTO);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    setAutoClimateState(AutoClimateState::OFF);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(autosar::HmiCmptmtAirDistbnFrnt::Flr, outSignal);
}

/**
 * GIVEN system is functioning normally and auto climate is enabled
 * WHEN  fan level front changes value
 * THEN  air distribution angle shall not change (be read from setting)
 */
TEST_F(AirDistributionTest, test_autoclimate_on_and_fan_level_change_does_not_change_angle) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAutoClimateState(AutoClimateState::ON);
    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL2);
    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());
}

/**
 * GIVEN system is functioning normally
 * WHEN  max defroster is enabled
 * THEN  air distribution angle shall be AUTO
 */
TEST_F(AirDistributionTest, test_maxdefroster_on_enables_auto_angle) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setMaxDefrosterValue(FirstRowGen::MaxDefrosterState::OFF);
    setAutoClimateState(AutoClimateState::OFF);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    setMaxDefrosterValue(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());

    setMaxDefrosterValue(FirstRowGen::MaxDefrosterState::OFF);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
}

/**
 * GIVEN system is functioning normally
 * WHEN  max defroster is disabled
 * THEN  air distribution angle shall be restored
 */
TEST_F(AirDistributionTest, test_maxdefroster_restores_previous_angle) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    airDistributionAngle_.set(AirDistributionAngle::AUTO);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL2);
    setAutoClimateState(AutoClimateState::OFF);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    setMaxDefrosterValue(FirstRowGen::MaxDefrosterState::OFF);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
}

/**
 * GIVEN that auto climate is on
 * WHEN fan level is set to off
 * THEN air distribution shall be set to AUTO (PA shall be set to disabled)
 */
TEST_F(AirDistributionTest, test_fanlevel_off_while_autoclimate_on_sets_auto) {
    autosar::HmiCmptmtAirDistbnFrnt outSignal(static_cast<autosar::HmiCmptmtAirDistbnFrnt>(-1));
    ECDDataElement::DESink<autosar::HmiCmptmtAirDistbnFrnt_info> AirDistributionSignal;
    AirDistributionSignal.subscribe(
            [&AirDistributionSignal, &outSignal]() { outSignal = AirDistributionSignal.get().value(); });

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setAutoClimateState(AutoClimateState::ON);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    // When FanLevel is set to off while Auto Climate is on, Air Distribution first receives
    // the Fanlevel OFF event followed by Auto Climate off.
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::OFF);
    setAutoClimateState(AutoClimateState::OFF);

    EXPECT_EQ(AirDistributionAngle::DISABLED, airDistributionAngle_.get());
    EXPECT_EQ(autosar::HmiCmptmtAirDistbnFrnt::Aut, outSignal);
}

/**
 * GIVEN system is functioning normally
 * WHEN climate reset is activated
 * THEN air distribution shall be set to AUTO
 */
TEST_F(AirDistributionTest, test_climate_reset_activated_sets_auto) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    shareClimateResetEvent.set(ClimateResetLogic::ClimateResetEvent::ACTIVATED);
    shareClimateResetEvent.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);

    EXPECT_EQ(AirDistributionAngle::AUTO, airDistributionAngle_.get());
}

/**
 * GIVEN system is functioning normally
 * WHEN when user switch profile
 * THEN air distribution shall be set to new profile value
 */
TEST_F(AirDistributionTest, test_climate_profile_change) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());

    sAirDistribution.set(AirDistributionAngle::FLOOR_VENT_DEFROST);

    EXPECT_EQ(AirDistributionAngle::FLOOR_VENT_DEFROST, airDistributionAngle_.get());
}

/**
 * GIVEN system is functioning normally
 * WHEN multiple vehicle mode signals are recived
 * THEN air distribution shall not spam flexray
 */
TEST_F(AirDistributionTest, test_spaming_of_vehicle_mode_will_not_make_air_dist_signal_spamming) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sAirDistribution.set(AirDistributionAngle::FLOOR);
    setAutoClimateState(AutoClimateState::OFF);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL2);

    ECDDataElement::DESink<autosar::HmiCmptmtAirDistbnFrnt_info> airDistSignal;
    HmiCmptmtAirDistbnFrnt outvalue = HmiCmptmtAirDistbnFrnt::Aut;
    airDistSignal.subscribe([&airDistSignal, &outvalue]() { outvalue = airDistSignal.get().value(); });

    AirDistributionLogic airDistributionLogic(airDistributionAngle_, sAirDistributionProxy, shareClimateResetEvent,
                                              shareFanLevel, shareAutoClimate, shareMaxDefroster);

    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(HmiCmptmtAirDistbnFrnt::Flr, outvalue);

    outvalue = HmiCmptmtAirDistbnFrnt::Aut;  // reset to auto so we can check if subscribe is called again

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(HmiCmptmtAirDistbnFrnt::Aut, outvalue);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(HmiCmptmtAirDistbnFrnt::Aut, outvalue);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(HmiCmptmtAirDistbnFrnt::Aut, outvalue);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(HmiCmptmtAirDistbnFrnt::Aut, outvalue);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(AirDistributionAngle::FLOOR, airDistributionAngle_.get());
    EXPECT_EQ(HmiCmptmtAirDistbnFrnt::Aut, outvalue);
}
}
