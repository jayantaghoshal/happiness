/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "seat_vent_logic.h"

#include "carconfig_mock.h"
#include "cedric_localconfig_mock.h"
#include "enum_helper.h"
#include "settings_proxy.h"

#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libsettings/setting.h>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include "mock_settingsmanager.h"

using namespace testing;

namespace TestSeatVent {

using CC8 = CarConfigParams::CC8_SteeringWheelPositionType;
using CC179 = CarConfigParams::CC179_FrontSeatVentilationType;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class SeatVentTest : public Test {
  public:
    SeatVentTest()
        : carConfig_{},
          settingsProxy{seatVentLevelSetting, seatVentLevelSettingDyno},
          shareVentAttribute{{FirstRowGen::VentState::DISABLED, FirstRowGen::VentLevel::OFF}},
          hmiSeatClimate_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC8::ParamNumber))).WillByDefault(Return(to_undrl(CC8::Left_Hand_Drive)));
        ON_CALL(carConfig_, getValue(to_undrl(CC179::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC179::Ventilated_seats)));

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

    std::unique_ptr<SeatVentLogic> makeSut(
            SeatVentLogic::UserLocation userLocation = SeatVentLogic::UserLocation::DRIVER) {
        resetSeatClimate();
        return std::make_unique<SeatVentLogic>(userLocation, hmiSeatClimate_, shareVentAttribute, settingsProxy);
    }

  protected:
    NiceMock<CarConfigMock> carConfig_;
    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    CompatSetting<FirstRowGen::VentLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED> seatVentLevelSetting{
            SettingId::TestSetting1, FirstRowGen::VentLevel::OFF, settingsManager};

    CompatSetting<FirstRowGen::VentLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED>
            seatVentLevelSettingDyno{SettingId::TestSetting1, FirstRowGen::VentLevel::OFF, settingsManager};

    SettingsProxy<FirstRowGen::VentLevel::Literal, SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            settingsProxy;

    NotifiableProperty<FirstRowGen::VentAttribute> shareVentAttribute;
    autosar::HmiSeatClima hmiSeatClimate_;
    std::unique_ptr<SeatVentLogic> sut_;
};

// User Request Testcases

TEST_F(SeatVentTest, Request_WillUpdateVentLevel_WhenHmiRequestMade) {
    sut_->request(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(FirstRowGen::VentLevel::LVL3, shareVentAttribute.get().getVentLevel());
}

// Can be removed if not needed - As it doesnt have Auto Mode. Test case is also for Auto Mode is not available
TEST_F(SeatVentTest, StateWillBeManual_WhenAutoStateIsOff) {
    EXPECT_EQ(FirstRowGen::VentState::MANUAL, shareVentAttribute.get().getVentState());
}

TEST_F(SeatVentTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnables) {
    sut_->request(FirstRowGen::VentLevel::LVL3);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);
    EXPECT_EQ(FirstRowGen::VentState::DISABLED, shareVentAttribute.get().getVentState());
    EXPECT_EQ(FirstRowGen::VentLevel::OFF, shareVentAttribute.get().getVentLevel());

    resetVehicleMode();
    EXPECT_EQ(FirstRowGen::VentState::MANUAL, shareVentAttribute.get().getVentState());
    EXPECT_EQ(FirstRowGen::VentLevel::LVL3, shareVentAttribute.get().getVentLevel());
}

// Error Signal

TEST_F(SeatVentTest, StateAndLevelDisabledAndOff_WhenSignalInError) {
    sut_->request(FirstRowGen::VentLevel::LVL3);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::VentState::MANUAL, shareVentAttribute.get().getVentState());
    EXPECT_EQ(FirstRowGen::VentLevel::LVL3, shareVentAttribute.get().getVentLevel());

    setVehicleModeError();

    EXPECT_EQ(FirstRowGen::VentState::DISABLED, shareVentAttribute.get().getVentState());
    EXPECT_EQ(FirstRowGen::VentLevel::OFF, shareVentAttribute.get().getVentLevel());
}

// Dyno Mode

TEST_F(SeatVentTest, StateAndLevelWillBeRestored_WhenActivationCheckReEnablesInDyno) {
    sut_->request(FirstRowGen::VentLevel::LVL3);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModTrnsp);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::VentState::MANUAL, shareVentAttribute.get().getVentState());
    EXPECT_EQ(FirstRowGen::VentLevel::OFF, shareVentAttribute.get().getVentLevel());
}

TEST_F(SeatVentTest, StateAndLevelWillNotChange_WhenTransitioningToDyno) {
    sut_->request(FirstRowGen::VentLevel::LVL3);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::VentState::MANUAL, shareVentAttribute.get().getVentState());
    EXPECT_EQ(FirstRowGen::VentLevel::OFF, shareVentAttribute.get().getVentLevel());
}

TEST_F(SeatVentTest, SettingWillBeWritten_WhenCarModeIsNotDyno) {
    sut_->request(FirstRowGen::VentLevel::LVL3);
    EXPECT_EQ(FirstRowGen::VentLevel::LVL3, seatVentLevelSetting.get());
}

TEST_F(SeatVentTest, SettingWillNotBeWritten_WhenCarModeIsDyno) {
    sut_->request(FirstRowGen::VentLevel::LVL3);
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    sut_->request(FirstRowGen::VentLevel::LVL1);
    EXPECT_EQ(FirstRowGen::VentLevel::LVL3, seatVentLevelSetting.get());
}

TEST_F(SeatVentTest, SimulateProfileChange) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    seatVentLevelSetting.set(FirstRowGen::VentLevel::OFF);

    EXPECT_EQ(FirstRowGen::VentLevel::OFF, seatVentLevelSetting.get());

    seatVentLevelSetting.set(FirstRowGen::VentLevel::LVL1);

    EXPECT_EQ(FirstRowGen::VentLevel::LVL1, seatVentLevelSetting.get());

    seatVentLevelSetting.set(FirstRowGen::VentLevel::LVL2);

    EXPECT_EQ(FirstRowGen::VentLevel::LVL2, seatVentLevelSetting.get());

    seatVentLevelSetting.set(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(FirstRowGen::VentLevel::LVL3, seatVentLevelSetting.get());
}

// Seat Vent Left hand driving TestCases

class SeatVentLeftHandDriveTest : public SeatVentTest {};

TEST_F(SeatVentLeftHandDriveTest, Request_WillSendDriverStateToLeftSignal_WhenSteeringWheelIsOnLeft) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatVentSignal;

    sut_->request(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatVentSignal.get().value().HmiSeatVentnForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatVentSignal.get().value().HmiSeatVentnForRowFirstRi);
}

TEST_F(SeatVentLeftHandDriveTest, Request_WillSendPassengerStateToRightSignal_WhenSteeringWheelIsOnLeft) {
    sut_ = makeSut(SeatVentLogic::UserLocation::PASSENGER);

    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatVentSignal;

    sut_->request(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatVentSignal.get().value().HmiSeatVentnForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatVentSignal.get().value().HmiSeatVentnForRowFirstRi);
}

// Seat Vent Right hand driving TestCases

class SeatVentRightHandDriveTest : public SeatVentTest {
  public:
    SeatVentRightHandDriveTest() : SeatVentTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC8::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC8::Right_Hand_Drive)));
    }
};

TEST_F(SeatVentRightHandDriveTest, Request_WillSendDriverStateToRightSignal_WhenSteeringWheelIsOnRight) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatVentSignal;

    sut_->request(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatVentSignal.get().value().HmiSeatVentnForRowFirstRi);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatVentSignal.get().value().HmiSeatVentnForRowFirstLe);
}

TEST_F(SeatVentRightHandDriveTest, Request_WillSendPassengerStateToLeftSignal_WhenSteeringWheelIsOnRight) {
    sut_ = makeSut(SeatVentLogic::UserLocation::PASSENGER);

    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatVentSignal;

    sut_->request(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatVentSignal.get().value().HmiSeatVentnForRowFirstRi);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatVentSignal.get().value().HmiSeatVentnForRowFirstLe);
}

// Seat Vntillation Activation Tests

class SeatVentActivationTest : public SeatVentTest,
                               public WithParamInterface<std::tuple<autosar::UsgModSts1, autosar::CarModSts1>> {};

TEST_P(SeatVentActivationTest, Constructor_WhenActivationCheckOk_WillActivate) {
    auto usgMode = std::get<0>(GetParam());
    auto carMode = std::get<1>(GetParam());

    setVehicleMode(usgMode, carMode);
    sut_->request(FirstRowGen::VentLevel::LVL1);

    auto carModeStatusOk = (carMode == autosar::CarModSts1::CarModNorm) || (carMode == autosar::CarModSts1::CarModDyno);

    auto activated = carModeStatusOk && (usgMode == autosar::UsgModSts1::UsgModDrvg);

    if (activated) {
        EXPECT_NE(FirstRowGen::VentState::DISABLED, shareVentAttribute.get().getVentState());
    } else {
        EXPECT_EQ(FirstRowGen::VentState::DISABLED, shareVentAttribute.get().getVentState());
    }
}

INSTANTIATE_TEST_CASE_P(, SeatVentActivationTest,
                        Combine(Values(autosar::UsgModSts1::UsgModAbdnd, autosar::UsgModSts1::UsgModInActv,
                                       autosar::UsgModSts1::UsgModCnvinc, autosar::UsgModSts1::UsgModActv,
                                       autosar::UsgModSts1::UsgModDrvg),
                                Values(autosar::CarModSts1::CarModNorm, autosar::CarModSts1::CarModTrnsp,
                                       autosar::CarModSts1::CarModFcy, autosar::CarModSts1::CarModCrash,
                                       autosar::CarModSts1::CarModDyno)));

// No Seat Ventillation Present - Car Config Invalid

class NoSeatVentillationTest : public SeatVentTest {
  public:
    NoSeatVentillationTest() : SeatVentTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC179::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC179::Without_ventilated_seats)));
    }
};

TEST_F(NoSeatVentillationTest, Constructor_WhenSeatVentillationNotPresent_WillNotActivate) {
    EXPECT_EQ(FirstRowGen::VentState::NOT_PRESENT, shareVentAttribute.get().getVentState());
}

TEST_F(NoSeatVentillationTest, Constructor_WhenCarConfigIsErroneous_WillThrow) {
    EXPECT_CALL(carConfig_, getValue(_)).WillRepeatedly(Throw(std::out_of_range{""}));

    EXPECT_THROW(makeSut(), std::out_of_range);
}

TEST_F(SeatVentTest, VentSignal_restore_old_value_after_usageMode_change) {
    ECDDataElement::DESink<autosar::HmiSeatClima_info> seatHeatSignal;

    std::unique_ptr<SeatVentLogic> driverLogic;
    std::unique_ptr<SeatVentLogic> passangerLogic;

    driverLogic = makeSut(SeatVentLogic::UserLocation::DRIVER);
    passangerLogic = makeSut(SeatVentLogic::UserLocation::PASSENGER);
    passangerLogic->request(FirstRowGen::VentLevel::LVL3);
    driverLogic->request(FirstRowGen::VentLevel::LVL3);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatVentnForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatVentnForRowFirstRi);

    setVehicleMode(autosar::UsgModSts1::UsgModInActv, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatVentnForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Off, seatHeatSignal.get().value().HmiSeatVentnForRowFirstRi);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatVentnForRowFirstLe);
    EXPECT_EQ(autosar::SeatClimaLvl::Lvl3, seatHeatSignal.get().value().HmiSeatVentnForRowFirstRi);
}
}
