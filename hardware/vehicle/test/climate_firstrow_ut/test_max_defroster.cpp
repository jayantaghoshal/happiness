/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <gtest/gtest.h>
#include "max_defroster_logic.h"
#include "mock_settingsmanager.h"

using namespace testing;

namespace TestMaxDefroster {

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class MaxDefrosterTest : public Test {
  protected:
    void setVehicleMode(autosar::UsgModSts1 const usgModeSts1, autosar::CarModSts1 const carModSts1) {
        autosar::VehModMngtGlbSafe1 const vehicleMode{
                usgModeSts1, carModSts1, 0, 0, 0, 0, 0, autosar::FltEgyCns1::NoFlt, 0, 0};
        ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal;
        vehicleModeSignal.inject(vehicleMode);
    }

    void resetVehicleMode() { setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModFcy); }

    void setClimaActive(autosar::OnOff1 onOff = autosar::OnOff1::Off) {
        ECDDataElement::DEInjector<autosar::ClimaActv_info> climaActiveSignal;
        climaActiveSignal.inject(onOff);
    }

    void TearDown() { resetVehicleMode(); }

  protected:
    NotifiableProperty<FirstRowGen::MaxDefrosterState> shareMaxDefroster{FirstRowGen::MaxDefrosterState::DISABLED};
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> shareFanLevelFront{FirstRowGen::FanLevelFrontValue::DISABLED};
    NotifiableProperty<FirstRowGen::AirDistributionAngle> shareAirDistribution{FirstRowGen::AirDistributionAngle::AUTO};
    NotifiableProperty<FirstRowGen::AutoClimateState> shareAutoClimate{FirstRowGen::AutoClimateState::OFF};
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> shareClimateReset{
            ClimateResetLogic::ClimateResetEvent::NO_REQ};
};

TEST_F(MaxDefrosterTest, test_request_from_hmi_updates_state) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();
    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    logic.request(FirstRowGen::MaxDefrosterRequest::ON);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::ON, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_request_from_hmi_sends_hmi_signal) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();
    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    ECDDataElement::DESink<autosar::HmiDefrstMaxReq_info> maxDefrosterSignal;

    logic.request(FirstRowGen::MaxDefrosterRequest::ON);
    EXPECT_EQ(autosar::ActrReq::On, maxDefrosterSignal.get().value());
}

TEST_F(MaxDefrosterTest, test_request_from_ccsm_updates_state_on) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();
    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    logic.request(OnOff::On);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::ON, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_request_from_ccsm_updates_state_off) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();
    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    logic.request(OnOff::On);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::ON, shareMaxDefroster.get());

    logic.request(OnOff::Off);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_change_to_carmode_dyno_turns_off_maxdefroster) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_change_to_carmode_dyno_turns_off_maxdefroster_late_signals) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    setClimaActive();

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_change_to_carmode_normal_turns_off_maxdefroster) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    setClimaActive();

    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_change_to_carmode_normal_turns_off_maxdefroster_late_signals) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_disabled_when_activation_check_fails) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    logic.request(FirstRowGen::MaxDefrosterRequest::ON);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::DISABLED, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_activation_and_decativation) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    setClimaActive(autosar::OnOff1::Off);

    setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::DISABLED, shareMaxDefroster.get());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());

    setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::DISABLED, shareMaxDefroster.get());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_check_when_usgmode_is_not_driving_and_clima_is_on) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm);
    setClimaActive(autosar::OnOff1::Off);

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::DISABLED, shareMaxDefroster.get());

    setClimaActive(autosar::OnOff1::On);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_turns_off_when_fan_level_is_off) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};
    shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);
    shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::OFF);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_turns_off_when_fan_level_is_in_error_state) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};
    shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);
    shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::SYSTEM_ERROR);

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_turns_off_when_fan_level_is_in_disabled_state) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};
    shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);
    shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::DISABLED);

    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_turns_off_when_air_distribution_is_not_auto) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};
    shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    shareAirDistribution.set(FirstRowGen::AirDistributionAngle::FLOOR);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_turns_off_when_auto_climte_is_on) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    logic.request(FirstRowGen::MaxDefrosterRequest::ON);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::ON, shareMaxDefroster.get());

    shareAutoClimate.set(FirstRowGen::AutoClimateState::ON);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

TEST_F(MaxDefrosterTest, test_max_defroster_turns_off_when_climete_reset_is_activated) {
    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    setClimaActive();

    logic.request(FirstRowGen::MaxDefrosterRequest::ON);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::ON, shareMaxDefroster.get());

    shareClimateReset.set(ClimateResetLogic::ClimateResetEvent::ACTIVATED);
    shareClimateReset.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    EXPECT_EQ(FirstRowGen::MaxDefrosterState::OFF, shareMaxDefroster.get());
}

class MaxDefrosterActivationTest
        : public MaxDefrosterTest,
          public WithParamInterface<std::tuple<FirstRowGen::MaxDefrosterState, FirstRowGen::MaxDefrosterRequest,
                                               autosar::UsgModSts1, autosar::CarModSts1, autosar::OnOff1>> {};

TEST_P(MaxDefrosterActivationTest, test_request_passes_fails_depending_on_activation_status) {
    auto expected = std::get<0>(GetParam());
    auto request = std::get<1>(GetParam());
    auto usageMode = std::get<2>(GetParam());
    auto carMode = std::get<3>(GetParam());
    auto climaActive = std::get<4>(GetParam());

    MaxDefrosterLogic logic{shareMaxDefroster, shareFanLevelFront, shareAirDistribution, shareAutoClimate,
                            shareClimateReset};

    setVehicleMode(usageMode, carMode);
    setClimaActive(climaActive);

    logic.request(request);

    EXPECT_EQ(expected, shareMaxDefroster.get());
}

INSTANTIATE_TEST_CASE_P(
        test_request_passes_fails_depending_on_activation_status, MaxDefrosterActivationTest,
        Values(std::make_tuple(FirstRowGen::MaxDefrosterState::DISABLED, FirstRowGen::MaxDefrosterRequest::ON,
                               autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm, autosar::OnOff1::Off),
               std::make_tuple(FirstRowGen::MaxDefrosterState::ON, FirstRowGen::MaxDefrosterRequest::ON,
                               autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm, autosar::OnOff1::Off),
               std::make_tuple(FirstRowGen::MaxDefrosterState::ON, FirstRowGen::MaxDefrosterRequest::ON,
                               autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno, autosar::OnOff1::Off),
               std::make_tuple(FirstRowGen::MaxDefrosterState::ON, FirstRowGen::MaxDefrosterRequest::ON,
                               autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModDyno, autosar::OnOff1::On),
               std::make_tuple(FirstRowGen::MaxDefrosterState::DISABLED, FirstRowGen::MaxDefrosterRequest::OFF,
                               autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModFcy, autosar::OnOff1::Off),
               std::make_tuple(FirstRowGen::MaxDefrosterState::DISABLED, FirstRowGen::MaxDefrosterRequest::OFF,
                               autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm,
                               autosar::OnOff1::Off)));
}
