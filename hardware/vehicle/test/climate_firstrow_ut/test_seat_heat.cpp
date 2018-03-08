/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vcc/localconfig.h>
#include "seat_heat_logic.h"

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

namespace TestSeatHeat {
using namespace std::chrono_literals;

auto const LCFG_DeterminationTimeout = std::string{"Climate_Determination_timeout"};
auto const lcfgTimeout = int{60};  // Measured in seconds
auto const timeout = std::chrono::seconds{lcfgTimeout};

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using UserSelectionGen = v0::org::volvocars::climate::UserSelection;
using CC8 = CarConfigParams::CC8_SteeringWheelPositionType;
using CC180 = CarConfigParams::CC180_HeatedFrontSeatsType;

class SeatHeatTest : public Test {
  public:
    SeatHeatTest()
        : autoSeatHeatOn{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}},
          autoSeatHeatLevel{{UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::LevelType::LO}},
          settingsProxy{seatHeatLevelSetting, seatHeatLevelSettingDyno},
          shareHeatAttribute{{FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF}},
          localConfig_{},
          hmiSeatClimate_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC8::ParamNumber))).WillByDefault(Return(to_undrl(CC8::Left_Hand_Drive)));
        ON_CALL(carConfig_, getValue(to_undrl(CC180::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC180::Heated_front_seats)));

        ON_CALL(localConfig_, GetIntMock(LCFG_DeterminationTimeout)).WillByDefault(Return(lcfgTimeout));

        DataElementFramework::instance().reset();
        setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
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

    void resetSeatClimate() {
        hmiSeatClimate_.HmiSeatHeatgForRowFirstLe = autosar::SeatClimaLvl::Off;
        hmiSeatClimate_.HmiSeatHeatgForRowFirstRi = autosar::SeatClimaLvl::Off;
        hmiSeatClimate_.HmiSeatHeatgForRowSecLe = autosar::SeatClimaLvl::Off;
        hmiSeatClimate_.HmiSeatHeatgForRowSecRi = autosar::SeatClimaLvl::Off;
        hmiSeatClimate_.HmiSeatVentnForRowFirstLe = autosar::SeatClimaLvl::Off;
        hmiSeatClimate_.HmiSeatVentnForRowFirstRi = autosar::SeatClimaLvl::Off;
        hmiSeatClimate_.HmiSeatHeatgForRowFirstLe = autosar::SeatClimaLvl::Off;
    }

    std::unique_ptr<SeatHeatLogic> makeSut(
            SeatHeatLogic::UserLocation userLocation = SeatHeatLogic::UserLocation::DRIVER) {
        resetSeatClimate();
        return std::make_unique<SeatHeatLogic>(&localConfig_, userLocation, hmiSeatClimate_, shareHeatAttribute,
                                               autoSeatHeatOn, autoSeatHeatLevel, settingsProxy, timeProvider_);
    }

    NotifiableProperty<UserSelectionGen::OffOnSelection> autoSeatHeatOn;
    NotifiableProperty<UserSelectionGen::LevelSelection> autoSeatHeatLevel;

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    CompatSetting<FirstRowGen::HeatLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED> seatHeatLevelSetting{
            SettingId::TestSetting1, FirstRowGen::HeatLevel::OFF, settingsManager};

    CompatSetting<FirstRowGen::HeatLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED>
            seatHeatLevelSettingDyno{SettingId::TestSetting2, FirstRowGen::HeatLevel::OFF, settingsManager};

    SettingsProxy<FirstRowGen::HeatLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            settingsProxy;

    NotifiableProperty<FirstRowGen::HeatAttribute> shareHeatAttribute;

    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig_;
    NiceMock<CarConfigMock> carConfig_;

    TimeProviderStub timeProvider_;
    autosar::HmiSeatClima hmiSeatClimate_;
    std::unique_ptr<SeatHeatLogic> sut_;
};

TEST_F(SeatHeatTest, Request_WillUpdateHeatLevel_WhenHmiRequestMade) {
    sut_->request(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatTest, StateWillBeManual_WhenAutoStateIsOff) {
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
}

TEST_F(SeatHeatTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnables) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);
    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    resetVehicleMode();
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatTest, StateAndLevelDisabledAndOff_WhenSignalInError) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    setVehicleModeError();

    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

/**
 * Handle cases where activationCheck fails twice in a row.
 *
 * As we are storing previous values we need to be careful to only
 * write to them when going _to_ an inactive state the first time. If
 * we don't, then we store the previous value again, overwriting the
 * actual value we had when we were active...  Trust me, it's possible...
 */
TEST_F(SeatHeatTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnablesAfterDoubleDeactivate) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);
    setVehicleMode(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModTrnsp);

    resetVehicleMode();
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnablesInDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatTest, StateAndLevelWillNotChange_WhenTransitioningToDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatTest, SettingWillBeWritten_WhenCarModeIsNotDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, seatHeatLevelSetting.get());
}

TEST_F(SeatHeatTest, SettingWillNotBeWritten_WhenCarModeIsDyno) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sut_->request(FirstRowGen::HeatLevel::LO);
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, seatHeatLevelSetting.get());
}

class SeatHeatLeftHandDriveTest : public SeatHeatTest {};

TEST_F(SeatHeatLeftHandDriveTest, Request_WillSendDriverStateToLeftSignal_WhenSteeringWheelIsOnLeft) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    sut_->request(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);
}

TEST_F(SeatHeatLeftHandDriveTest, Request_WillSendPassengerStateToRightSignal_WhenSteeringWheelIsOnLeft) {
    sut_ = makeSut(SeatHeatLogic::UserLocation::PASSENGER);

    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    sut_->request(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);
}

class SeatHeatRightHandDriveTest : public SeatHeatTest {
  public:
    SeatHeatRightHandDriveTest() : SeatHeatTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC8::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC8::Right_Hand_Drive)));
    }
};

TEST_F(SeatHeatRightHandDriveTest, Request_WillSendDriverStateToRightSignal_WhenSteeringWheelIsOnRight) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    sut_->request(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
}

TEST_F(SeatHeatRightHandDriveTest, Request_WillSendPassengerStateToLeftSignal_WhenSteeringWheelIsOnRight) {
    sut_ = makeSut(SeatHeatLogic::UserLocation::PASSENGER);

    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    sut_->request(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
}

class SeatHeatActivationTest : public SeatHeatTest,
                               public WithParamInterface<std::tuple<autosar::UsgModSts1, autosar::CarModSts1>> {};

TEST_P(SeatHeatActivationTest, Constructor_WhenActivationCheckOk_WillActivate) {
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

INSTANTIATE_TEST_CASE_P(, SeatHeatActivationTest,
                        Combine(Values(autosar::UsgModSts1::UsgModAbdnd, autosar::UsgModSts1::UsgModInActv,
                                       autosar::UsgModSts1::UsgModCnvinc, autosar::UsgModSts1::UsgModActv,
                                       autosar::UsgModSts1::UsgModDrvg),
                                Values(autosar::CarModSts1::CarModNorm, autosar::CarModSts1::CarModTrnsp,
                                       autosar::CarModSts1::CarModFcy, autosar::CarModSts1::CarModCrash,
                                       autosar::CarModSts1::CarModDyno)));

class NoSeatHeatersPresentTest : public SeatHeatTest {
  public:
    NoSeatHeatersPresentTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC180::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC180::Without_heated_front_seats)));

        sut_ = makeSut();
    }
};

TEST_F(NoSeatHeatersPresentTest, Constructor_WhenHeatedSeatsNotPresent_WillNotActivate) {
    EXPECT_EQ(FirstRowGen::HeatState::NOT_PRESENT, shareHeatAttribute.get().getHeatState());
}

TEST_F(NoSeatHeatersPresentTest, Request_WhenHeatedSeatsNotPresent_WillNotSendSignal) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
}

TEST_F(NoSeatHeatersPresentTest, Request_WhenHeatedSeatsNotPresent_WillNotUpdateNotifiableProperty) {
    sut_->request(FirstRowGen::HeatLevel::HI);
    EXPECT_EQ(FirstRowGen::HeatState::NOT_PRESENT, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(NoSeatHeatersPresentTest, Constructor_WhenCarConfigIsErroneous_WillThrow) {
    EXPECT_CALL(carConfig_, getValue(_)).WillRepeatedly(Throw(std::out_of_range{""}));

    EXPECT_THROW(makeSut(), std::out_of_range);
}

class SeatHeatAutoTest : public SeatHeatTest {
  public:
    SeatHeatAutoTest() {
        autoSeatHeatOn.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
        autoSeatHeatLevel.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::LevelType::LO});
        sut_ = makeSut();
    }

    void injectAutoCdnSignal(autosar::Flg1 flag = autosar::Flg1::Set) const {
        ECDDataElement::DEInjector<autosar::SeatHeatgAutCdn_info>().inject(flag);
    }
};

TEST_F(SeatHeatAutoTest, HeatAttribute_WhenAutoSettingOn_WillBeAutoOn) {
    timeProvider_.sleep_for(timeout - 1ms);
    injectAutoCdnSignal();
    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatAutoTest, HeatAttribute_WhenAutoSettingOn_WillBeAutoOnAfterEveryActivation) {
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

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SeatHeatAutoTest, DISABLED_HeatAttribute_WhenAutoAutoCdnNotSet_WillBeAutoOff) {
    injectAutoCdnSignal(autosar::Flg1::Rst);
    sut_ = makeSut();
    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SeatHeatAutoTest, DISABLED_HeatAttribute_WhenWaitTimesOut_WillBeAutoOff) {
    sut_ = makeSut();
    timeProvider_.sleep_for(timeout);

    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SeatHeatAutoTest, DISABLED_HeatAttribute_WhenSignalArrivesAfterTimeout_WillBeAutoOff) {
    sut_ = makeSut();

    timeProvider_.sleep_for(timeout);
    injectAutoCdnSignal();
    EXPECT_EQ(FirstRowGen::HeatState::AUTO, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatAutoTest, HeatAttribute_WhenCarModeIsDyno_WillBeManualOff) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sut_ = makeSut();
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatAutoTest, HeatAttribute_WhenCarModeIsDynoAndAutoConditionSignalIsSet_WillBeManualOff) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sut_ = makeSut();
    timeProvider_.sleep_for(timeout - std::chrono::milliseconds{1});
    injectAutoCdnSignal();
    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatAutoTest, SetAutoCondition_WhenNotActive_WillBeDisabled) {
    seatHeatLevelSetting.set(FirstRowGen::HeatLevel::HI);
    sut_ = makeSut(SeatHeatLogic::UserLocation::DRIVER);

    injectAutoCdnSignal(autosar::Flg1::Rst);
    setVehicleMode(autosar::UsgModSts1::UsgModCnvinc, autosar::CarModSts1::CarModNorm);
    injectAutoCdnSignal(autosar::Flg1::Set);

    EXPECT_EQ(FirstRowGen::HeatState::DISABLED, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());
}

TEST_F(SeatHeatTest, SimulateProfileChange) {
    seatHeatLevelSetting.set(FirstRowGen::HeatLevel::OFF);

    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    seatHeatLevelSetting.set(FirstRowGen::HeatLevel::LO);

    EXPECT_EQ(FirstRowGen::HeatLevel::LO, shareHeatAttribute.get().getHeatLevel());

    seatHeatLevelSetting.set(FirstRowGen::HeatLevel::MED);

    EXPECT_EQ(FirstRowGen::HeatLevel::MED, shareHeatAttribute.get().getHeatLevel());

    seatHeatLevelSetting.set(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SeatHeatTest, DISABLED_HeatAttribute_PassengerSide_WhenCarModeSwitchBetweenDynoAndNormal_WillBeManualOff) {
    seatHeatLevelSetting.set(FirstRowGen::HeatLevel::HI);
    sut_ = makeSut(SeatHeatLogic::UserLocation::PASSENGER);

    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);

    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::OFF, shareHeatAttribute.get().getHeatLevel());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(FirstRowGen::HeatState::MANUAL, shareHeatAttribute.get().getHeatState());
    EXPECT_EQ(FirstRowGen::HeatLevel::HI, shareHeatAttribute.get().getHeatLevel());
}

// TODO(ARTINFO-503): Enable test case, see implementation for comment with failure details.
TEST_F(SeatHeatTest, DISABLED_HeatSignal_restore_old_value_after_usageMode_change) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    std::unique_ptr<SeatHeatLogic> driverLogic;
    std::unique_ptr<SeatHeatLogic> passangerLogic;

    driverLogic = makeSut(SeatHeatLogic::UserLocation::DRIVER);
    passangerLogic = makeSut(SeatHeatLogic::UserLocation::PASSENGER);
    passangerLogic->request(FirstRowGen::HeatLevel::HI);
    driverLogic->request(FirstRowGen::HeatLevel::HI);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);

    setVehicleMode(autosar::UsgModSts1::UsgModInActv, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatHeatgForRowFirstRi);
}
}
