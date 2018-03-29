/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "auto_climate_logic.h"

#include "air_distribution_logic.h"
#include "carconfig_mock.h"
#include "enum_helper.h"
#include "fan_level_front_logic.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>

namespace TestAirConditioner {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace testing;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using CC175 = CarConfigParams::CC175_HvacVariantsType;

class AutoClimateTest : public Test {
  public:
    AutoClimateTest()
        : carConfig_{},
          shareAirDistribution{FirstRowGen::AirDistributionAngle::DISABLED},
          shareAutoClimate{FirstRowGen::AutoClimateState::DISABLED},
          shareAutoClimateEvent{AutoClimateLogic::AutoClimateEvent::NO_REQ},
          shareFanLevelFront{FirstRowGen::FanLevelFrontValue::DISABLED},
          shareMaxDefroster{FirstRowGen::MaxDefrosterState::DISABLED} {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC175::Electronic_Climate_Control_2_zone)));
    }

    ~AutoClimateTest() { resetVehicleMode(); }

  protected:
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

    void setConditionsOk(bool ok) {
        if (ok) {
            shareAirDistribution.set(FirstRowGen::AirDistributionAngle::AUTO);
            shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::OFF);
            shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::LVL1);
        } else {
            shareAirDistribution.set(FirstRowGen::AirDistributionAngle::FLOOR);
            shareMaxDefroster.set(FirstRowGen::MaxDefrosterState::ON);
            shareFanLevelFront.set(FirstRowGen::FanLevelFrontValue::OFF);
        }
    }

    NiceMock<CarConfigMock> carConfig_;

    NotifiableProperty<FirstRowGen::AirDistributionAngle> shareAirDistribution;
    NotifiableProperty<FirstRowGen::AutoClimateState> shareAutoClimate;
    NotifiableProperty<AutoClimateLogic::AutoClimateEvent> shareAutoClimateEvent;
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> shareFanLevelFront;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> shareMaxDefroster;

    ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
};

TEST_F(AutoClimateTest, test_car_config_valid) {
    {
        AutoClimateLogic ac(
                shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

        // Go to non-active state. Usage mode - driving and car mode - factory
        setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy);

        EXPECT_EQ(FirstRowGen::AutoClimateState::DISABLED, shareAutoClimate.get());
    }
    {
        EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillRepeatedly(Return(to_undrl(CC175::Electronic_Climate_Control_4_zone)));

        AutoClimateLogic ac(
                shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

        // Go to non-active state. Usage mode - driving and car mode - factory
        setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy);

        EXPECT_EQ(FirstRowGen::AutoClimateState::DISABLED, shareAutoClimate.get());
    }
    {
        EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillRepeatedly(Return(to_undrl(CC175::HVAC_Small_1_zone)));

        AutoClimateLogic ac(
                shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

        // Go to non-active state. Usage mode - driving and car mode - factory
        setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy);

        EXPECT_EQ(FirstRowGen::AutoClimateState::DISABLED, shareAutoClimate.get());
    }
    {
        EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillRepeatedly(Return(to_undrl(CC175::HVAC_Small_2_zone)));

        AutoClimateLogic ac(
                shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

        // Go to non-active state. Usage mode - driving and car mode - factory
        setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy);

        EXPECT_EQ(FirstRowGen::AutoClimateState::DISABLED, shareAutoClimate.get());
    }
}

TEST_F(AutoClimateTest, test_car_config_invalid) {
    {
        EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillRepeatedly(Return(to_undrl(CC175::HVAC_Small_3_zone)));

        AutoClimateLogic ac(
                shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

        EXPECT_EQ(FirstRowGen::AutoClimateState::NOT_VISIBLE, shareAutoClimate.get());
    }
    {
        EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillRepeatedly(Return(to_undrl(CC175::Manual_AC)));

        AutoClimateLogic ac(
                shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

        EXPECT_EQ(FirstRowGen::AutoClimateState::NOT_VISIBLE, shareAutoClimate.get());
    }
}

TEST_F(AutoClimateTest, test_conditions_ok_usage_mode_driving_car_mode_normal) {
    AutoClimateLogic ac(
            shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

    // Go to active state. Usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    // Set other functions to fulfill the requirements for auto climate to be on
    setConditionsOk(true);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());
}

TEST_F(AutoClimateTest, test_conditions_ok_usage_mode_driving_car_mode_dyno) {
    AutoClimateLogic ac(
            shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

    // Go to active state. Usage mode - driving and car mode - dyno
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    // Set other functions to fulfill the requirements for auto climate to be on
    setConditionsOk(true);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());
}

TEST_F(AutoClimateTest, test_car_mode_changing) {
    AutoClimateLogic ac(
            shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

    // Go to active state. Usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    // Set other functions to fulfill the requirements for auto climate to be on
    setConditionsOk(true);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());

    // Go to non-active state. Usage mode - driving and car mode - factory
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy);

    EXPECT_EQ(FirstRowGen::AutoClimateState::DISABLED, shareAutoClimate.get());

    // Go to active state. Usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());

    // Stay in active state. Usage mode - driving and car mode - dyno
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());
}

TEST_F(AutoClimateTest, test_multiple_car_mode_driving) {
    AutoClimateLogic ac(
            shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

    // Go to active state. Usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    // Set other functions to fulfill the requirements for auto climate to be on
    setConditionsOk(true);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());

    // Resend usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());
}

TEST_F(AutoClimateTest, test_conditions_changing) {
    AutoClimateLogic ac(
            shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

    // Go to active state. Usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setConditionsOk(false);

    EXPECT_EQ(FirstRowGen::AutoClimateState::OFF, shareAutoClimate.get());

    // Set other functions to fulfill the requirements for auto climate to be on
    setConditionsOk(true);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());

    // Set other functions to not fulfill the requirements for auto climate to be on
    setConditionsOk(false);

    EXPECT_EQ(FirstRowGen::AutoClimateState::OFF, shareAutoClimate.get());

    setConditionsOk(true);

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());
}

TEST_F(AutoClimateTest, test_auto_climate_request) {
    AutoClimateLogic ac(
            shareAutoClimate, shareAutoClimateEvent, shareAirDistribution, shareFanLevelFront, shareMaxDefroster);

    // Go to active state. Usage mode - driving and car mode - normal
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    // Set other functions to fulfill the requirements for auto climate to be off
    setConditionsOk(false);

    EXPECT_EQ(FirstRowGen::AutoClimateState::OFF, shareAutoClimate.get());

    // Creating a lamdba to verify event outputs from auto climate request
    int autoClimateEventNo = 1;

    auto verifyEvents = [this, &autoClimateEventNo](const auto&) {
        EXPECT_TRUE(autoClimateEventNo == 1 || autoClimateEventNo == 2);

        if (autoClimateEventNo == 1) {
            EXPECT_EQ(shareAutoClimateEvent.get(), AutoClimateLogic::AutoClimateEvent::ACTIVATED);
            ++autoClimateEventNo;
        } else if (autoClimateEventNo == 2) {
            EXPECT_EQ(shareAutoClimateEvent.get(), AutoClimateLogic::AutoClimateEvent::NO_REQ);
        }
    };

    shareAutoClimateEvent.subscribe(verifyEvents);

    ac.requestAutoClimate();

    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());

    // Simulate the result in the other climate functions
    setConditionsOk(true);

    // Let's request again when auto climate already is on
    autoClimateEventNo = 1;

    ac.requestAutoClimate();
    EXPECT_EQ(FirstRowGen::AutoClimateState::ON, shareAutoClimate.get());
}
}
