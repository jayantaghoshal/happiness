/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vcc/localconfig.h>
#include "steering_wheel_heat_logic.h"

#include "carconfig_mock.h"

#include "enum_helper.h"
#include "settings_proxy.h"
#include "time_provider_stub.h"

#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libsettings/setting.h>
#include <chrono>
#include <string>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "mock_settingsmanager.h"

using namespace testing;

namespace TestSteeringWhlHeat {
using namespace std::chrono_literals;

auto const LCFG_DeterminationTimeout = std::string{"Climate_Determination_timeout"};
auto const lcfgTimeout = int{60};  // Measured in seconds
auto const timeout = std::chrono::seconds{lcfgTimeout};
auto const defaultTimeout = 20s;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using CC186 = CarConfigParams::CC186_HeatedSteeringWheelType;

class SteeringWheelHeatTest : public Test {
  public:
    SteeringWheelHeatTest()
        : settingsProxy{steeringWheelHeatLevelSetting, steeringWheelHeatLevelSettingDyno},
          shareHeatAttribute{{FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF}},
          autoSteeringWheelHeatOn({UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}),
          autoSteeringWheelHeatLevel({UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::LevelType::LO}),
          localConfig_{},
          carConfig_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC186::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC186::Heated_steering_wheel)));

        ON_CALL(localConfig_, GetIntMock(LCFG_DeterminationTimeout)).WillByDefault(Return(lcfgTimeout));

        DataElementFramework::instance().reset();
        setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
        steeringWheelHeatLevelSetting.set(FirstRowGen::HeatLevel::OFF);
    }

    void SetUp() { sut_ = makeSut(); }

  protected:
    static void setVehicleMode(autosar::UsgModSts1 const usgModeSts1, autosar::CarModSts1 const carModSts1) {
        autosar::VehModMngtGlbSafe1 const vehicleMode{
                usgModeSts1, carModSts1, 0, 0, 0, 0, 0, autosar::FltEgyCns1::NoFlt, 0, 0};
        ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal;
        vehicleModeSignal.inject(vehicleMode);
    }

    static void setVehicleModeError() {
        ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal;
        vehicleModeSignal.error(1);
    }

    static void resetVehicleMode() { setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm); }

    std::unique_ptr<SteeringWheelHeatLogic> makeSut() {
        return std::make_unique<SteeringWheelHeatLogic>(&localConfig_,
                                                        shareHeatAttribute,
                                                        autoSteeringWheelHeatOn,
                                                        autoSteeringWheelHeatLevel,
                                                        settingsProxy,
                                                        timeProvider_);
    }

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    CompatSetting<FirstRowGen::HeatLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED>
            steeringWheelHeatLevelSetting{SettingId::TestSetting1, FirstRowGen::HeatLevel::OFF, settingsManager};

    CompatSetting<FirstRowGen::HeatLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED>
            steeringWheelHeatLevelSettingDyno{SettingId::TestSetting2, FirstRowGen::HeatLevel::OFF, settingsManager};

    SettingsProxy<FirstRowGen::HeatLevel::Literal,
                  SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            settingsProxy;

    NotifiableProperty<FirstRowGen::HeatAttribute> shareHeatAttribute;

    NotifiableProperty<UserSelectionGen::OffOnSelection> autoSteeringWheelHeatOn;
    NotifiableProperty<UserSelectionGen::LevelSelection> autoSteeringWheelHeatLevel;

    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig_;
    NiceMock<CarConfigMock> carConfig_;

    TimeProviderStub timeProvider_;
    std::unique_ptr<SteeringWheelHeatLogic> sut_;
};

TEST_F(SteeringWheelHeatTest, Request_WillUpdateSteeringWheelHeatLevel_WhenHmiRequestMade) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::MED);
    EXPECT_EQ(FirstRowGen::HeatLevel::MED, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatTest, StateWillBeManual_WhenSteeringWheelAutoStateIsOff) {
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
}

TEST_F(SteeringWheelHeatTest, StateAndLevelWillBeRestored_WhenSteeringWheelActivationCheckOKReEnables) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);
    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    resetVehicleMode();
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatTest, StateAndLevelDisabledAndOff_WhenVehicleModeSignalInError) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    setVehicleModeError();

    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

// Handle cases where activationCheckOk fails twice in a row.

TEST_F(SteeringWheelHeatTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnablesAfterDoubleDeactivate) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);

    resetVehicleMode();
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnablesInDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatTest, Level_WillSetToOff_WhenTransitioningToDyno) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatTest, SettingWillBeWritten_WhenCarModeIsNotDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, steeringWheelHeatLevelSetting.get());
}

TEST_F(SteeringWheelHeatTest, SettingsWillNotBeWritten_WhenCarModeIsDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sut_->request(FirstRowGen::HeatLevel::LO);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, steeringWheelHeatLevelSetting.get());
}

TEST_F(SteeringWheelHeatTest, SimulateProfileChange) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    steeringWheelHeatLevelSetting.set(FirstRowGen::HeatLevel::OFF);

    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, steeringWheelHeatLevelSetting.get());

    steeringWheelHeatLevelSetting.set(FirstRowGen::HeatLevel::LO);

    EXPECT_EQ(FirstRowGen::HeatLevel::LO, steeringWheelHeatLevelSetting.get());

    steeringWheelHeatLevelSetting.set(FirstRowGen::HeatLevel::MED);

    EXPECT_EQ(FirstRowGen::HeatLevel::MED, steeringWheelHeatLevelSetting.get());

    steeringWheelHeatLevelSetting.set(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(FirstRowGen::HeatLevel::HI, steeringWheelHeatLevelSetting.get());
}

// Handle Steering Wheel Heat Activation Test
class SteeringWheelHeatActivationTest
        : public SteeringWheelHeatTest,
          public WithParamInterface<std::tuple<autosar::UsgModSts1, autosar::CarModSts1>> {};

TEST_P(SteeringWheelHeatActivationTest, Constructor_WhenActivationCheckOk_WillActivate) {
    auto usgMode = std::get<0>(GetParam());
    auto carMode = std::get<1>(GetParam());

    setVehicleMode(usgMode, carMode);
    sut_->request(FirstRowGen::HeatLevel::LO);

    auto carModeStatusOk = (carMode == autosar::CarModSts1::CarModNorm) || (carMode == autosar::CarModSts1::CarModDyno);

    auto activated = carModeStatusOk && (usgMode == autosar::UsgModSts1::UsgModDrvg);

    if (activated) {
        EXPECT_NE(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    } else {
        EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    }
}

INSTANTIATE_TEST_CASE_P(,
                        SteeringWheelHeatActivationTest,
                        Combine(Values(autosar::UsgModSts1::UsgModAbdnd,
                                       autosar::UsgModSts1::UsgModInActv,
                                       autosar::UsgModSts1::UsgModCnvinc,
                                       autosar::UsgModSts1::UsgModActv,
                                       autosar::UsgModSts1::UsgModDrvg),
                                Values(autosar::CarModSts1::CarModNorm,
                                       autosar::CarModSts1::CarModTrnsp,
                                       autosar::CarModSts1::CarModFcy,
                                       autosar::CarModSts1::CarModCrash,
                                       autosar::CarModSts1::CarModDyno)));

class NoSteeringWheelHeatersPresentTest : public SteeringWheelHeatTest {
  public:
    NoSteeringWheelHeatersPresentTest() : SteeringWheelHeatTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC186::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC186::Without_heated_steering_wheel)));
    }
};

TEST_F(NoSteeringWheelHeatersPresentTest, Constructor_WhenHeatedSteeringWheelNotPresent_WillNotActivate) {
    EXPECT_EQ(FirstRowGen::HeatState::NOT_PRESENT, shareHeatAttribute.get().getHeatState());
}

TEST_F(NoSteeringWheelHeatersPresentTest, Request_WhenHeatedSteeringWheelNotPresent_WillNotSendSignal) {
    autosar::SteerWhlHeatgOnCmdTyp outSignal(autosar::SteerWhlHeatgOnCmdTyp::Off);
    bool called = false;
    ECDDataElement::DESink<autosar::SteerWhlHeatgOnReq_info> steeringWheelHeatingSignal;

    steeringWheelHeatingSignal.subscribe([&steeringWheelHeatingSignal, &outSignal, &called]() {
        if (steeringWheelHeatingSignal.get().isOk()) {
            outSignal = steeringWheelHeatingSignal.get().value();
            called = true;
        }
    });

    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(false, called);
    EXPECT_EQ(autosar::SteerWhlHeatgOnCmdTyp::Off, outSignal);
}

TEST_F(NoSteeringWheelHeatersPresentTest, Constructor_WhenCarConfigIsErroneous_WillThrow) {
    EXPECT_CALL(carConfig_, getValue(to_undrl(CC186::ParamNumber))).WillRepeatedly(Throw(std::out_of_range{""}));

    EXPECT_THROW(makeSut(), std::out_of_range);
}

class SteeringWheelHeatAutoTest : public SteeringWheelHeatTest {
  public:
    SteeringWheelHeatAutoTest() {
        autoSteeringWheelHeatOn.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
        autoSteeringWheelHeatLevel.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::LevelType::LO});
        sut_ = makeSut();
    }

    void injectAutoCdnSignal(autosar::OnOff1 state = autosar::OnOff1::On) const {
        ECDDataElement::DEInjector<autosar::SteerWhlHeatgStrtAutCdnOk_info>().inject(state);
    }
};

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SteeringWheelHeatAutoTest, DISABLED_HeatAttribute_WhenAutoAutoCdnNotSet_WillBeAutoOff_ThenDoManualSetting) {
    injectAutoCdnSignal(autosar::OnOff1::Off);
    sut_ = makeSut();
    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::MED);
    EXPECT_EQ(FirstRowGen::HeatLevel::MED, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::OFF);
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SteeringWheelHeatAutoTest,
       DISABLED_HeatAttribute_WhenWaitTimesOut_WillBeAutoOff_ThenDoManualSetting_AgainSettoAuto) {
    sut_ = makeSut();
    timeProvider_.sleep_for(timeout);

    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    // HMI Request
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::MED);
    EXPECT_EQ(FirstRowGen::HeatLevel::MED, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::OFF);
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    // Again setting to Auto Mode
    sut_ = makeSut();
    timeProvider_.sleep_for(defaultTimeout);
    injectAutoCdnSignal();

    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatAutoTest, HeatAttribute_WhenAutoSettingOn_WillBeAutoOn_ThenDoManualSetting_AgainSettoAuto) {
    timeProvider_.sleep_for(timeout - 1ms);
    injectAutoCdnSignal();
    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());

    // HMI Request
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::MED);
    EXPECT_EQ(FirstRowGen::HeatLevel::MED, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    sut_->request(FirstRowGen::HeatLevel::OFF);
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    // Again setting to Auto Mode
    sut_ = makeSut();
    timeProvider_.sleep_for(defaultTimeout);
    injectAutoCdnSignal();

    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SteeringWheelHeatAutoTest, HeatAttribute_WhenAutoSettingOn_WillBeAutoOnAfterEveryActivation) {
    timeProvider_.sleep_for(timeout - 1ms);
    injectAutoCdnSignal();
    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());

    setVehicleMode(autosar::UsgModSts1::UsgModCnvinc, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());

    setVehicleMode(autosar::UsgModSts1::UsgModCnvinc, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}
}
