/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <chrono>
#include "mock_dispatcher.h"
#include "mock_settingsmanager.h"
#include "vfc_driving_climatisation.h"

namespace TestVFCClimatisation {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace ECDDataElement;
using namespace testing;

class VFCClimatisationTest : public ::testing::Test {
  public:
    DEInjector<autosar::VehModMngtGlbSafe1_info> vehMode;

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1_;
    ApplicationDataElement::DESender<autosar::HmiHvacFanLvlFrnt_info> levelOfFanLevelFront_;
    ApplicationDataElement::DESender<autosar::HmiSeatClima_info> seatHeatAndVent_;
    ApplicationDataElement::DESender<autosar::HmiSeatClimaExtd_info> seatVentEx_;
    ApplicationDataElement::DESender<ActivateVfc_info> activatevfc_;

    NiceMock<MockDispatcher> dispatcher_;

    /*==============================================*/
    /*               Help functions                 */
    /*==============================================*/

    VFCClimatisationTest() : dispatcher_() { DataElementFramework::instance().reset(); }

    ~VFCClimatisationTest() {}

    void SetUp() {
        resetVehicleMode();
        activatevfc_.send({Vfc::Crash, 255});
    }

    void TearDown() { resetVehicleMode(); }

    void setVehicleMode(UsgModSts1 usgModeSts1, CarModSts1 carModSts1) {
        VehModMngtGlbSafe1 mode;
        mode.UsgModSts = usgModeSts1;
        mode.CarModSts1_ = carModSts1;
        vehMode.inject(mode);
    }

    void resetVehicleMode() {
        VehModMngtGlbSafe1 mode;
        mode.CarModSts1_ = CarModSts1::CarModFcy;
        mode.UsgModSts = UsgModSts1::UsgModAbdnd;
        mode.CarModSubtypWdCarModSubtyp = 0;
        mode.EgyLvlElecMai = 0;
        mode.EgyLvlElecSubtyp = 0;
        mode.PwrLvlElecMai = 0;
        mode.PwrLvlElecSubtyp = 0;
        mode.FltEgyCnsWdSts = FltEgyCns1::NoFlt;
        mode.Chks = 0;
        mode.Cntr = 0;
        vehMode.inject(mode);
    }

    void setVehicleModeError() { vehMode.error(12); }
};

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(VFCClimatisationTest, test_instantiate) { VFCDrivingClimatisationLogic vfc{dispatcher_}; }

TEST_F(VFCClimatisationTest, test_that_vfc_should_be_default_off) {
    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
}

class VFCClimatisationNotActiveTest : public VFCClimatisationTest, public WithParamInterface<UsgModSts1> {};

TEST_P(VFCClimatisationNotActiveTest, test_that_vfc_should_be_off_if_not_in_driving) {
    auto usgMode = GetParam();
    setVehicleMode(usgMode, CarModSts1::CarModCrash);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 0};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
    EXPECT_EQ(255, outvalue.secondsToKeepActive);
}

INSTANTIATE_TEST_CASE_P(test_that_vfc_should_be_off_if_not_in_driving, VFCClimatisationNotActiveTest,
                        Values(UsgModSts1::UsgModAbdnd, UsgModSts1::UsgModInActv, UsgModSts1::UsgModCnvinc,
                               UsgModSts1::UsgModActv));

TEST_F(VFCClimatisationTest, test_that_vfc_should_be_on_when_in_driving_and_fan_is_not_off) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);
    levelOfFanLevelFront_.send(HmiHvacFanLvl::LvlAutoNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);
    EXPECT_EQ(40, outvalue.secondsToKeepActive);
}

TEST_F(VFCClimatisationTest, test_that_vfc_should_be_off_when_in_driving_and_fan_is_off) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_vfc_should_be_off_when_in_driving_and_fan_is_turned_off) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_vfc_should_be_on_when_in_driving_and_fan_is_turned_on) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};
    levelOfFanLevelFront_.send(HmiHvacFanLvl::LvlAutPlusPlus);

    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_vfc_should_be_retriggerd_after_timout) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    std::function<void(void)> func;

    VFCDrivingClimatisationLogic vfc{dispatcher_};
    EXPECT_CALL(dispatcher_, Start(_, _, false)).WillRepeatedly(SaveArg<1>(&func));

    levelOfFanLevelFront_.send(HmiHvacFanLvl::LvlAutPlusPlus);
    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);

    // Clear the signal so we can see if it is triggerd again
    activatevfc_.send({Vfc::Crash, 255});
    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);

    EXPECT_CALL(dispatcher_, Restart(_, false)).Times(2);
    // Now we have a timeout
    func();

    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);

    // Clear the signal so we can see if it is triggerd again
    activatevfc_.send({Vfc::Crash, 255});
    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);

    // Now we have a timeout
    func();

    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_timer_is_off_if_not_in_driving) {
    EXPECT_CALL(dispatcher_, Start(_, _, _)).Times(0);
    setVehicleMode(UsgModSts1::UsgModActv, CarModSts1::CarModCrash);

    VFCDrivingClimatisationLogic vfc{dispatcher_};
}

TEST_F(VFCClimatisationTest, test_that_vf_is_on_when_all_seat_heat_is_on_and_in_driving) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);

    HmiSeatClima heat{SeatClimaLvl::Lvl1, SeatClimaLvl::Lvl1, SeatClimaLvl::Lvl1,
                      SeatClimaLvl::Lvl1, SeatClimaLvl::Lvl1, SeatClimaLvl::Lvl1};

    seatHeatAndVent_.send(heat);

    HmiSeatClimaExtd heatEx{SeatClimaLvl::Lvl1, SeatClimaLvl::Lvl1};

    seatVentEx_.send(heatEx);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_vf_is_on_when_one_seat_heat_is_on_and_in_driving) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);

    HmiSeatClima heat{SeatClimaLvl::Off, SeatClimaLvl::Lvl1, SeatClimaLvl::Off,
                      SeatClimaLvl::Off, SeatClimaLvl::Off,  SeatClimaLvl::Off};

    seatHeatAndVent_.send(heat);

    HmiSeatClimaExtd heatEx{SeatClimaLvl::Off, SeatClimaLvl::Off};

    seatVentEx_.send(heatEx);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_vf_is_off_when_no_seat_heat_is_on_and_in_driving) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);

    HmiSeatClima heat{SeatClimaLvl::Off, SeatClimaLvl::Off, SeatClimaLvl::Off,
                      SeatClimaLvl::Off, SeatClimaLvl::Off, SeatClimaLvl::Off};

    seatHeatAndVent_.send(heat);

    HmiSeatClimaExtd heatEx{SeatClimaLvl::Off, SeatClimaLvl::Off};

    seatVentEx_.send(heatEx);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
}

TEST_F(VFCClimatisationTest, test_that_vf_not_resent_within_30_secs) {
    VFCDrivingClimatisationLogic vfc{dispatcher_};
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    auto nofCalls = 0;
    vfcSink.subscribe([&nofCalls]() { nofCalls++; });

    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);

    EXPECT_EQ(1, nofCalls);
}

TEST_F(VFCClimatisationTest, signalCallback_WhenVFCTurnsOff_WillSendDeactivateSignal) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCDrivingClimatisationLogic vfc{dispatcher_};

    levelOfFanLevelFront_.send(HmiHvacFanLvl::LvlAutPlusPlus);
    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);

    ON_CALL(dispatcher_, IsRunning()).WillByDefault(Return(true));
    levelOfFanLevelFront_.send(HmiHvacFanLvl::Off);
    EXPECT_EQ(Vfc::DrivingClimatisation, outvalue.vfcToActivate);
    EXPECT_EQ(0, outvalue.secondsToKeepActive);
}
}
