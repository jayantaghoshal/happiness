/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cleanzone_logic.h"

#include "carconfig_mock.h"
#include "enum_helper.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace TestCleanzone {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace ECDDataElement;
using namespace testing;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using CC174 = CarConfigParams::CC174_AirQualitySystemType;

class CleanZoneTest : public Test {
  public:
    CleanZoneTest() : cleanZoneIndication_(FirstRowGen::CleanzoneState::NOT_OK) {
        ON_CALL(carConfig_, getValue(to_undrl(CC174::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC174::Air_Quality_System)));

        resetVehicleMode();
        setLevelOfComfort(LvlOfClimaCmft::Off);
        setClimateActive(OnOff1::Off);
    }

    ~CleanZoneTest() {
        resetVehicleMode();
        setLevelOfComfort(LvlOfClimaCmft::Off);
        setClimateActive(OnOff1::Off);
    }

  protected:
    void setVehicleMode(UsgModSts1 usgModeSts1, CarModSts1 carModSts1, bool injectError = 0) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        drvMode.CarModSts1_ = carModSts1;
        if (injectError)
            driverMode.error(12);
        else
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

    void setLevelOfComfort(LvlOfClimaCmft lvlOfClimaCmft, bool injectError = 0) {
        DEInjector<autosar::LvlOfClimaCmft_info> levelOfClimateComfort;
        if (injectError)
            levelOfClimateComfort.error(12);
        else
            levelOfClimateComfort.inject(lvlOfClimaCmft);
    }

    void setClimateActive(OnOff1 climateActiveValue, bool injectError = 0) {
        DEInjector<autosar::ClimaActv_info> climateActive;
        if (injectError)
            climateActive.error(12);
        else
            climateActive.inject(climateActiveValue);
    }

    NiceMock<CarConfigMock> carConfig_;

    NotifiableProperty<FirstRowGen::CleanzoneState> cleanZoneIndication_;
    DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
};

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

using CZState = CleanZoneLogic::CleanZoneIndicationState;

// Testing all values of CarConfig
class CleanzoneCarConfigTest : public CleanZoneTest, public WithParamInterface<std::tr1::tuple<CC174, CZState>> {};

TEST_P(CleanzoneCarConfigTest, testCarConfig) {
    auto carConfigValue = std::get<0>(GetParam());
    auto state = std::get<1>(GetParam());

    EXPECT_CALL(carConfig_, getValue(to_undrl(CC174::ParamNumber))).WillRepeatedly(Return(to_undrl(carConfigValue)));

    CleanZoneLogic cz(cleanZoneIndication_);

    EXPECT_EQ(cleanZoneIndication_.get(), state);
}

INSTANTIATE_TEST_CASE_P(testCarConfig,
                        CleanzoneCarConfigTest,
                        Values(std::tr1::tuple<CC174, CZState>(CC174::Air_Quality_System, CZState::NOT_OK),
                               std::tr1::tuple<CC174, CZState>(CC174::InvalidValue, CZState::OFF),
                               std::tr1::tuple<CC174, CZState>(CC174::No_Air_Quality_System, CZState::OFF)));

// Testing all values of LevelOfClimateComfort
class CleanzoneLevelOfComfortTest : public CleanZoneTest,
                                    public WithParamInterface<std::tr1::tuple<bool, LvlOfClimaCmft, CZState>> {};

TEST_P(CleanzoneLevelOfComfortTest, testLevelOfComfort) {
    auto order = std::get<0>(GetParam());
    auto level = std::get<1>(GetParam());
    auto state = std::get<2>(GetParam());

    CleanZoneLogic cz(cleanZoneIndication_);

    setClimateActive(OnOff1::On);

    if (order) {
        setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
        setLevelOfComfort(level);
    } else {
        setLevelOfComfort(level);
        setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    }

    EXPECT_EQ(cleanZoneIndication_.get(), state);
}
INSTANTIATE_TEST_CASE_P(
        testLevelOfComfort,
        CleanzoneLevelOfComfortTest,
        Values(std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Off, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl1, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl2, CZState::OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl3, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl4, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl5, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl6, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(false, LvlOfClimaCmft::Lvl7, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Off, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl1, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl2, CZState::OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl3, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl4, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl5, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl6, CZState::NOT_OK),
               std::tr1::tuple<bool, LvlOfClimaCmft, CZState>(true, LvlOfClimaCmft::Lvl7, CZState::NOT_OK)));

// Testing all values of UsageMode
class CleanzoneVehicleModesTest
        : public CleanZoneTest,
          public WithParamInterface<std::tr1::tuple<UsgModSts1, CarModSts1, OnOff1, LvlOfClimaCmft, bool, bool, bool>> {
};

TEST_P(CleanzoneVehicleModesTest, testVehicleModes) {
    auto usageMode = std::get<0>(GetParam());
    auto carMode = std::get<1>(GetParam());
    auto climateActive = std::get<2>(GetParam());
    auto levelOfComfort = std::get<3>(GetParam());
    auto injectVehError = std::get<4>(GetParam());
    auto injectComfortError = std::get<5>(GetParam());
    auto injectClimateActiveError = std::get<6>(GetParam());

    CleanZoneLogic cz(cleanZoneIndication_);
    setVehicleMode(usageMode, carMode, injectVehError);
    setLevelOfComfort(levelOfComfort, injectComfortError);
    setClimateActive(climateActive, injectClimateActiveError);

    auto levelOfComfortCheck = (levelOfComfort == LvlOfClimaCmft::Lvl2);

    auto activationcheck = ((usageMode == UsgModSts1::UsgModDrvg ||
                             (climateActive == OnOff1::On && usageMode != UsgModSts1::UsgModAbdnd)) &&
                            (carMode == CarModSts1::CarModNorm || carMode == CarModSts1::CarModDyno));

    if (injectVehError || injectComfortError || injectClimateActiveError) {
        EXPECT_EQ(cleanZoneIndication_.get(), CZState::NOT_OK);
    } else {
        if (activationcheck) {
            if (levelOfComfortCheck) {
                EXPECT_EQ(cleanZoneIndication_.get(), CZState::OK);
            } else {
                EXPECT_EQ(cleanZoneIndication_.get(), CZState::NOT_OK);
            }
        } else {
            EXPECT_EQ(cleanZoneIndication_.get(), CZState::NOT_OK);
        }
    }
}
INSTANTIATE_TEST_CASE_P(testVehicleModes,
                        CleanzoneVehicleModesTest,
                        Combine(Values(UsgModSts1::UsgModAbdnd,
                                       UsgModSts1::UsgModActv,
                                       UsgModSts1::UsgModCnvinc,
                                       UsgModSts1::UsgModDrvg,
                                       UsgModSts1::UsgModInActv),
                                Values(CarModSts1::CarModCrash,
                                       CarModSts1::CarModDyno,
                                       CarModSts1::CarModFcy,
                                       CarModSts1::CarModNorm,
                                       CarModSts1::CarModTrnsp),
                                Values(OnOff1::Off, OnOff1::On),
                                Values(LvlOfClimaCmft::Off,
                                       LvlOfClimaCmft::Lvl1,
                                       LvlOfClimaCmft::Lvl2,
                                       LvlOfClimaCmft::Lvl3,
                                       LvlOfClimaCmft::Lvl4,
                                       LvlOfClimaCmft::Lvl5,
                                       LvlOfClimaCmft::Lvl6,
                                       LvlOfClimaCmft::Lvl7),
                                Values(false, true),
                                Values(false, true),
                                Values(false, true)));

}  // namespace TestCleanzone
