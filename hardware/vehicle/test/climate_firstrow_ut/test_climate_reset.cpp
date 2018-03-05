/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_reset_logic.h"

#include "carconfig_mock.h"
#include "enum_helper.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gtest/gtest.h>

namespace TestClimateReset {

using namespace testing;
using namespace ApplicationDataElement;
using namespace autosar;

using CC175 = CarConfigParams::CC175_HvacVariantsType;

class ClimateResetTest : public ::testing::Test {
  public:
    ClimateResetTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC175::Electronic_Climate_Control_2_zone)));

        resetParkingClimate();
        resetVehicleMode();
    }

    ~ClimateResetTest() { resetVehicleMode(); }

  protected:
    void setVehicleMode(UsgModSts1 usgModeSts1, CarModSts1 carModSts1) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        drvMode.CarModSts1_ = carModSts1;
        driverMode_.inject(drvMode);
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
        driverMode_.inject(drvMode);
    }

    void setParkingClimate(OnOff1 val) { parkingClimate_.inject(val); }

    void resetParkingClimate() { parkingClimate_.inject(OnOff1::Off); }

    NiceMock<CarConfigMock> carConfig_;

    ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode_;
    ECDDataElement::DEInjector<autosar::ClimaActv_info> parkingClimate_;
};

TEST_F(ClimateResetTest, test_initialize_climate_reset) {
    // Go to non-active state. Usage mode - driving and car mode - factory
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> event;
    event.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    ClimateResetLogic climateReset{event};

    int climateResetEventNo = 0;

    auto verifyEvents = [this, &event, &climateResetEventNo](const auto&) {
        EXPECT_TRUE(climateResetEventNo == 0 || climateResetEventNo == 1);
        if (climateResetEventNo == 0) {
            EXPECT_EQ(event.get(), ClimateResetLogic::ClimateResetEvent::ACTIVATED);
            ++climateResetEventNo;
        } else if (climateResetEventNo == 1) {
            EXPECT_EQ(event.get(), ClimateResetLogic::ClimateResetEvent::NO_REQ);
            ++climateResetEventNo;
        }
    };

    auto id = event.subscribe(verifyEvents);
    climateReset.request();
    id.reset();
    EXPECT_EQ(climateResetEventNo, 2);
}

class ClimateResetActivationTest
        : public ClimateResetTest,
          public WithParamInterface<std::tuple<autosar::UsgModSts1, autosar::CarModSts1, autosar::OnOff1>> {};

TEST_P(ClimateResetActivationTest, test_activation_of_climate_reset_fails) {
    auto usageMode = std::get<0>(GetParam());
    auto carMode = std::get<1>(GetParam());
    auto climaActive = std::get<2>(GetParam());

    setVehicleMode(usageMode, carMode);
    setParkingClimate(climaActive);

    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> event;
    event.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    ClimateResetLogic climateReset{event};

    bool triggerd = false;
    auto id = event.subscribe([this, &event, &triggerd](const auto&) {

        if (event.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) triggerd = true;
    });

    climateReset.request();
    id.reset();

    EXPECT_FALSE(triggerd);
}

INSTANTIATE_TEST_CASE_P(
        test_activation_of_climate_reset_fails, ClimateResetActivationTest,
        Values(std::make_tuple(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm, autosar::OnOff1::Off),
               std::make_tuple(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModDyno, autosar::OnOff1::On),
               std::make_tuple(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModFcy, autosar::OnOff1::Off),
               std::make_tuple(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm,
                               autosar::OnOff1::Off)));

class ClimateResetDeActivationTest
        : public ClimateResetTest,
          public WithParamInterface<std::tuple<autosar::UsgModSts1, autosar::CarModSts1, autosar::OnOff1>> {};

TEST_P(ClimateResetDeActivationTest, test_activation_of_climate_reset_is_ok) {
    auto usageMode = std::get<0>(GetParam());
    auto carMode = std::get<1>(GetParam());
    auto climaActive = std::get<2>(GetParam());

    setVehicleMode(usageMode, carMode);
    setParkingClimate(climaActive);

    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> event;
    event.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    ClimateResetLogic climateReset{event};

    bool triggerd = false;
    auto id = event.subscribe([this, &event, &triggerd](const auto&) {

        if (event.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) triggerd = true;
    });

    climateReset.request();
    id.reset();

    EXPECT_TRUE(triggerd);
}

INSTANTIATE_TEST_CASE_P(
        test_activation_of_climate_reset_is_ok, ClimateResetDeActivationTest,
        Values(std::make_tuple(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno, autosar::OnOff1::Off),
               std::make_tuple(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm, autosar::OnOff1::Off),
               std::make_tuple(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno, autosar::OnOff1::On),
               std::make_tuple(autosar::UsgModSts1::UsgModActv, autosar::CarModSts1::CarModNorm, autosar::OnOff1::On),
               std::make_tuple(autosar::UsgModSts1::UsgModCnvinc, autosar::CarModSts1::CarModDyno, autosar::OnOff1::On)

                       ));

class ClimateResetNoCCTest : public ClimateResetTest,
                             public WithParamInterface<CarConfigParams::CC175_HvacVariantsType> {};

TEST_P(ClimateResetNoCCTest, test_activation_of_climate_reset_fails_if_no_valid_carconfig) {
    EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillRepeatedly(Return(to_undrl(GetParam())));

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> event;
    event.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    ClimateResetLogic climateReset{event};

    bool triggerd = false;
    auto id = event.subscribe([this, &event, &triggerd](const auto&) {

        if (event.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) triggerd = true;
    });

    climateReset.request();
    id.reset();

    EXPECT_FALSE(triggerd);
}

INSTANTIATE_TEST_CASE_P(test_activation_of_climate_reset_is_ok, ClimateResetNoCCTest,
                        Values(0, CarConfigParams::CC175_HvacVariantsType::HVAC_Small_3_zone,
                               CarConfigParams::CC175_HvacVariantsType::Manual_AC,
                               CarConfigParams::CC175_HvacVariantsType::InvalidValue));

class ClimateResetValidCCTest : public ClimateResetTest,
                                public WithParamInterface<CarConfigParams::CC175_HvacVariantsType> {};

TEST_P(ClimateResetValidCCTest, test_activation_of_climate_reset_fails_if_no_valid_carconfig) {
    EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillRepeatedly(Return(to_undrl(GetParam())));

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> event;
    event.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    ClimateResetLogic climateReset{event};

    bool triggerd = false;
    auto id = event.subscribe([this, &event, &triggerd](const auto&) {

        if (event.get() == ClimateResetLogic::ClimateResetEvent::ACTIVATED) triggerd = true;
    });

    climateReset.request();
    id.reset();

    EXPECT_TRUE(triggerd);
}

INSTANTIATE_TEST_CASE_P(test_activation_of_climate_reset_is_ok, ClimateResetValidCCTest,
                        Values(CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_2_zone,
                               CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_4_zone,
                               CarConfigParams::CC175_HvacVariantsType::HVAC_Small_1_zone,
                               CarConfigParams::CC175_HvacVariantsType::HVAC_Small_2_zone));
}
