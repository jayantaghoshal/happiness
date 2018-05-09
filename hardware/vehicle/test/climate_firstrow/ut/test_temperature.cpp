/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_logic.h"

#include "carconfig_mock.h"
#include "cc_parameterlist.h"
#include "enum_helper.h"
#include "temperature_converter.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include <tuple>
#include "mock_settingsmanager.h"

namespace TestTemperature {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace ECDDataElement;
using namespace testing;
using namespace SettingsFramework;

using CC175 = CarConfigParams::CC175_HvacVariantsType;

class TemperatureTest : public ::testing::Test {
  public:
    TemperatureTest()
        : carConfig_{},
          tempConverter_{},
          convertedTemp_{20.0},
          storedTemp_{22.0},
          storedTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
          temp_{20.0},
          tempHiLoN_{autosar::HmiCmptmtTSpSpcl::Hi},
          state_{FirstRowGen::StateType::NOT_PRESENT},
          fanLevelFront_{FirstRowGen::FanLevelFrontValue::OFF},
          maxDefroster_{FirstRowGen::MaxDefrosterState::OFF},
          climateResetEvent_{ClimateResetLogic::ClimateResetEvent::NO_REQ},
          indcdUnit_(autosar::AmbTIndcdUnit::Celsius),
          sut_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC175::Electronic_Climate_Control_2_zone)));

        resetVehicleMode();
        climateActive_.inject(OnOff1::On);
        setTUnit(AmbTIndcdUnit::Celsius);

        resetSignals();
    }

    void SetUp() override { createSut(); }

  protected:
    static void setVehicleMode(UsgModSts1 usgModeSts1, CarModSts1 carModSts1) {
        VehModMngtGlbSafe1 mode;
        mode.UsgModSts = usgModeSts1;
        mode.CarModSts1_ = carModSts1;
        vehModeInjector_.inject(mode);
    }

    static void setTUnit(autosar::AmbTIndcdUnit tUnit) {
        IndcnUnit indcnUnit;
        indcnUnit.TUnit = tUnit;
        indcnUnit_.send(indcnUnit);
    }

    static void resetVehicleMode() {
        VehModMngtGlbSafe1 mode;
        mode.CarModSts1_ = CarModSts1::CarModNorm;
        mode.UsgModSts = UsgModSts1::UsgModDrvg;
        mode.CarModSubtypWdCarModSubtyp = 0;
        mode.EgyLvlElecMai = 0;
        mode.EgyLvlElecSubtyp = 0;
        mode.PwrLvlElecMai = 0;
        mode.PwrLvlElecSubtyp = 0;
        mode.FltEgyCnsWdSts = FltEgyCns1::NoFlt;
        mode.Chks = 0;
        mode.Cntr = 0;
        vehModeInjector_.inject(mode);
    }

    static void setVehicleModeError() { vehModeInjector_.error(12); }

    void createSut() {
        sut_ = std::make_unique<TemperatureLogic>(true,
                                                  tempConverter_,
                                                  convertedTemp_,
                                                  storedTemp_,
                                                  storedTempHiLoN_,
                                                  temp_,
                                                  tempHiLoN_,
                                                  state_,
                                                  fanLevelFront_,
                                                  maxDefroster_,
                                                  climateResetEvent_);
    }

    void resetSignals() {
        storedTemp_.set(22.0);
        storedTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        temp_.set(20.0);
        tempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);
        state_.set(FirstRowGen::StateType::NOT_PRESENT);
        fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::OFF);
        maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
        climateResetEvent_.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
    }

  protected:
    static DEInjector<autosar::VehModMngtGlbSafe1_info> vehModeInjector_;
    static DEInjector<autosar::ClimaActv_info> climateActive_;
    static DESender<autosar::IndcnUnit_info> indcnUnit_;
    static ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;
    static ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActiveSignal_;

    NiceMock<CarConfigMock> carConfig_;

    common::daemon::TemperatureConverter tempConverter_;
    NotifiableProperty<double> convertedTemp_;
    NotifiableProperty<double> storedTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> storedTempHiLoN_;
    NotifiableProperty<double> temp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> tempHiLoN_;
    NotifiableProperty<FirstRowGen::StateType> state_;
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> fanLevelFront_;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> maxDefroster_;
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> climateResetEvent_;

    autosar::AmbTIndcdUnit indcdUnit_;

    std::unique_ptr<TemperatureLogic> sut_;
};

DEInjector<autosar::VehModMngtGlbSafe1_info> TemperatureTest::vehModeInjector_;
DEInjector<autosar::ClimaActv_info> TemperatureTest::climateActive_;
DESender<autosar::IndcnUnit_info> TemperatureTest::indcnUnit_;
ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> TemperatureTest::vehModMngtGlbSafe1_;
ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> TemperatureTest::climateActiveSignal_;

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(TemperatureTest, Constructor_WillInitialize) {
    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::OFF);
    resetSignals();

    TemperatureLogic sut(true,
                         tempConverter_,
                         convertedTemp_,
                         storedTemp_,
                         storedTempHiLoN_,
                         temp_,
                         tempHiLoN_,
                         state_,
                         fanLevelFront_,
                         maxDefroster_,
                         climateResetEvent_);

    EXPECT_DOUBLE_EQ(22.0, storedTemp_.get());
    EXPECT_DOUBLE_EQ(storedTemp_.get(), temp_.get());

    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, storedTempHiLoN_.get());
    EXPECT_EQ(storedTempHiLoN_.get(), tempHiLoN_.get());
    EXPECT_EQ(FirstRowGen::StateType::DISABLED, state_.get());
}

TEST_F(TemperatureTest, Constructor_WhenCarConfigNotOk_WillBeNotPresent) {
    TemperatureLogic sut(false,
                         tempConverter_,
                         convertedTemp_,
                         storedTemp_,
                         storedTempHiLoN_,
                         temp_,
                         tempHiLoN_,
                         state_,
                         fanLevelFront_,
                         maxDefroster_,
                         climateResetEvent_);

    EXPECT_EQ(FirstRowGen::StateType::NOT_PRESENT, state_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActivationOkAndFanOff_WillSetDisabled) {
    EXPECT_EQ(FirstRowGen::StateType::DISABLED, state_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActivationOkAndFanOn_WillSetAvailable) {
    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::LVL1);

    TemperatureLogic sut(true,
                         tempConverter_,
                         convertedTemp_,
                         storedTemp_,
                         storedTempHiLoN_,
                         temp_,
                         tempHiLoN_,
                         state_,
                         fanLevelFront_,
                         maxDefroster_,
                         climateResetEvent_);

    EXPECT_EQ(FirstRowGen::StateType::AVAILABLE, state_.get());
}

TEST_F(TemperatureTest, Constructor_WhenTempChanged_WillUpdateConvertedTemp) {
    auto expectedValue = 25.0;

    temp_.set(expectedValue);

    EXPECT_DOUBLE_EQ(tempConverter_.toSingle(indcdUnit_, expectedValue), convertedTemp_.get());
}

// TEST_F(TemperatureTest, Constructor_WhenTempHiLoNChanged_WillUpdateConvertedTemp) {
//     auto expectedValue = autosar::HmiCmptmtTSpSpcl::Hi;

//     tempHiLoN_.set(expectedValue);

//     EXPECT_DOUBLE_EQ(tempConverter_.toSingle(indcdUnit_, temp_.get(), expectedValue), convertedTemp_.get());
// }

TEST_F(TemperatureTest, Constructor_WhenActiveStoredTempChanged_WillUpdateTemp) {
    auto expectedValue = 25.0;

    storedTemp_.set(expectedValue);

    EXPECT_DOUBLE_EQ(expectedValue, temp_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActiveStoredTempHiLoNChanged_WillUpdateTempHiLoN) {
    auto expectedValue = autosar::HmiCmptmtTSpSpcl::Hi;

    storedTempHiLoN_.set(expectedValue);

    EXPECT_EQ(expectedValue, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Constructor_WhenClimateResetActivated_WillReset) {
    storedTemp_.set(27.5);
    storedTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    climateResetEvent_.set(ClimateResetLogic::ClimateResetEvent::ACTIVATED);

    EXPECT_DOUBLE_EQ(22.0, temp_.get());
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActiveMaxDefrosterChangedOn_WillSetTempHi) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Hi, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActiveMaxDefrosterChangedOff_WillRestoreTempAndTempHiLoN) {
    auto temp = temp_.get();
    auto tempHiLoN = tempHiLoN_.get();

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);

    sut_->request(16.5);

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);

    EXPECT_DOUBLE_EQ(temp, temp_.get());
    EXPECT_EQ(tempHiLoN, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActiveMaxDefrosterChangedOffToDisabled_WillNotRestoreTempAndTempHiLoN) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);

    sut_->request(16.5);

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::DISABLED);

    EXPECT_DOUBLE_EQ(16.5, temp_.get());
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Lo, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActiveAndFanLevelOff_WillSetDisabled) {
    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::LVL1);
    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::OFF);

    EXPECT_EQ(FirstRowGen::StateType::DISABLED, state_.get());
}

TEST_F(TemperatureTest, Constructor_WhenActiveAndFanLevelOn_WillSetAvailable) {
    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::OFF);
    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::LVL1);

    EXPECT_EQ(FirstRowGen::StateType::AVAILABLE, state_.get());
}

// TEST_F(TemperatureTest, Constructor_WhenTUnitChanged_WillUpdateConvertedTemp) {
//     setTUnit(AmbTIndcdUnit::Fahrenheit);

//     EXPECT_DOUBLE_EQ(tempConverter_.toSingle(AmbTIndcdUnit::Fahrenheit, temp_.get(), tempHiLoN_.get()),
//                      convertedTemp_.get());
// }

TEST_F(TemperatureTest, Request_WhenNotActive_WillNotUpdateTemp) {
    setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm);

    auto expectedValue = 25.0;

    sut_->request(expectedValue);

    EXPECT_DOUBLE_EQ(22, temp_.get());
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Request_WhenTempChanged_WillUpdateTemp) {
    auto expectedValue = 25.0;

    sut_->request(expectedValue);

    EXPECT_DOUBLE_EQ(expectedValue, temp_.get());
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Request_WhenTempOutOfRangeLo_WillRoundToTempLo) {
    sut_->request(16);

    EXPECT_DOUBLE_EQ(16.5, temp_.get());
}

TEST_F(TemperatureTest, Request_WhenTempOutOfRangeHi_WillRoundToTempHi) {
    sut_->request(28);

    EXPECT_DOUBLE_EQ(27.5, temp_.get());
}

TEST_F(TemperatureTest, Request_WhenTempLow_WillUpdateTempHiLoN) {
    sut_->request(16.5);

    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Lo, tempHiLoN_.get());
}

TEST_F(TemperatureTest, Request_WhenTempHi_WillUpdateTempHiLoN) {
    sut_->request(27.5);

    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Hi, tempHiLoN_.get());
}

// TEST_F(TemperatureTest, RequestRaw_WhenActive_WillUpdateTemp) {
//     sut_->request(25, autosar::HmiCmptmtTSpSpcl::Hi);

//     EXPECT_DOUBLE_EQ(25, temp_.get());
//     EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Hi, tempHiLoN_.get());
// }

// TEST_F(TemperatureTest, RequestRaw_WhenNotActive_WillUpdateTemp) {
//     setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModNorm);

//     sut_->request(25, autosar::HmiCmptmtTSpSpcl::Hi);

//     EXPECT_DOUBLE_EQ(22, temp_.get());
//     EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, tempHiLoN_.get());
// }

class TemperaturePassengerCarConfigTest : public TemperatureTest,
                                          public WithParamInterface<CarConfigParams::CC175_HvacVariantsType> {};

TEST_P(TemperaturePassengerCarConfigTest, IsPassengerCarConfigValid_WhenValid_WillReturnTrue) {
    auto param = GetParam();

    EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillRepeatedly(Return(to_undrl(param)));

    std::array<CarConfigParams::CC175_HvacVariantsType, 3> validValues{{
            CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_2_zone,
            CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_4_zone,
            CarConfigParams::CC175_HvacVariantsType::HVAC_Small_2_zone,
    }};

    auto valid = std::find(std::begin(validValues), std::end(validValues), param) != std::end(validValues);

    EXPECT_EQ(valid, TemperatureLogic::isPassengerCarConfigValid());
}

INSTANTIATE_TEST_CASE_P(IsPassengerCarConfigValid_WhenValid_WillReturnTrue,
                        TemperaturePassengerCarConfigTest,
                        Values(CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_2_zone,
                               CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_4_zone,
                               CarConfigParams::CC175_HvacVariantsType::HVAC_Small_1_zone,
                               CarConfigParams::CC175_HvacVariantsType::HVAC_Small_2_zone,
                               CarConfigParams::CC175_HvacVariantsType::HVAC_Small_3_zone,
                               CarConfigParams::CC175_HvacVariantsType::Manual_AC,
                               CarConfigParams::CC175_HvacVariantsType::InvalidValue));

class TemperatureActivationTest : public TemperatureTest,
                                  public WithParamInterface<std::tuple<UsgModSts1, CarModSts1, OnOff1>> {};

TEST_P(TemperatureActivationTest, Constructor_WillActivate_WhenOk) {
    auto usgMode = std::get<0>(GetParam());
    auto carMode = std::get<1>(GetParam());
    auto climate = std::get<2>(GetParam());

    fanLevelFront_.set(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(usgMode, carMode);

    climateActive_.inject(climate);

    auto carModeStatusOk = (carMode == CarModSts1::CarModNorm) || (carMode == CarModSts1::CarModDyno);

    auto result = carModeStatusOk && ((usgMode == UsgModSts1::UsgModDrvg) ||
                                      ((climate == OnOff1::On) && (usgMode != UsgModSts1::UsgModAbdnd)));

    auto expectedState = FirstRowGen::StateType::DISABLED;
    if (result) {
        expectedState = FirstRowGen::StateType::AVAILABLE;
    }

    EXPECT_EQ(expectedState, state_.get());
}

INSTANTIATE_TEST_CASE_P(Constructor_WillActivate_WhenOk,
                        TemperatureActivationTest,
                        Combine(Values(UsgModSts1::UsgModAbdnd,
                                       UsgModSts1::UsgModInActv,
                                       UsgModSts1::UsgModCnvinc,
                                       UsgModSts1::UsgModActv,
                                       UsgModSts1::UsgModDrvg),
                                Values(CarModSts1::CarModNorm,
                                       CarModSts1::CarModTrnsp,
                                       CarModSts1::CarModFcy,
                                       CarModSts1::CarModCrash,
                                       CarModSts1::CarModDyno),
                                Values(OnOff1::Off, OnOff1::On)));
}
