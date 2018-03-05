/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <gtest/gtest.h>
#include "mock_settingsmanager.h"
#include "vfc_visibility.h"

namespace TestVFCVisibility {

using namespace ApplicationDataElement;
using namespace ECDDataElement;
using namespace testing;

class VFCVisibilityTest : public ::testing::Test {
  public:
    DEInjector<autosar::VehModMngtGlbSafe1_info> vehMode;
    DEInjector<autosar::Btn4ForUsrSwtPanFrntReq_info> btn4_;
    DEInjector<autosar::Btn5ForUsrSwtPanFrntReq_info> btn5_;
    ApplicationDataElement::DESender<ActivateVfc_info> activatevfc_;

    /*==============================================*/
    /*               Help functions                 */
    /*==============================================*/

    VFCVisibilityTest() {
        resetVehicleMode();
        activatevfc_.send({Vfc::Crash, 255});
        btn4_.inject(autosar::PsdNotPsd::NotPsd);
        btn5_.inject(autosar::PsdNotPsd::NotPsd);
    }

    ~VFCVisibilityTest() {}

    void SetUp() {}

    void TearDown() { resetVehicleMode(); }

    void setVehicleMode(autosar::UsgModSts1 usgModeSts1, autosar::CarModSts1 carModSts1) {
        autosar::VehModMngtGlbSafe1 mode;
        mode.UsgModSts = usgModeSts1;
        mode.CarModSts1_ = carModSts1;
        vehMode.inject(mode);
    }

    void resetVehicleMode() {
        autosar::VehModMngtGlbSafe1 mode;
        mode.CarModSts1_ = autosar::CarModSts1::CarModFcy;
        mode.UsgModSts = autosar::UsgModSts1::UsgModAbdnd;
        mode.CarModSubtypWdCarModSubtyp = 0;
        mode.EgyLvlElecMai = 0;
        mode.EgyLvlElecSubtyp = 0;
        mode.PwrLvlElecMai = 0;
        mode.PwrLvlElecSubtyp = 0;
        mode.FltEgyCnsWdSts = autosar::FltEgyCns1::NoFlt;
        mode.Chks = 0;
        mode.Cntr = 0;
        vehMode.inject(mode);
    }

    void setVehicleModeError() { vehMode.error(12); }
};

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(VFCVisibilityTest, test_instantiate) { VFCVisibilityLogic vfc{}; }

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_default_off) {
    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
}

class VFCVisibilityNotActiveTest : public VFCVisibilityTest, public WithParamInterface<autosar::UsgModSts1> {};

TEST_P(VFCVisibilityNotActiveTest, test_that_vfc_should_be_off_if_not_in_driving) {
    auto usgMode = GetParam();
    setVehicleMode(usgMode, autosar::CarModSts1::CarModNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 0};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    btn4_.inject(autosar::PsdNotPsd::Psd);
    btn5_.inject(autosar::PsdNotPsd::Psd);

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
    EXPECT_EQ(255, outvalue.secondsToKeepActive);
}

INSTANTIATE_TEST_CASE_P(test_that_vfc_should_be_off_if_not_in_driving, VFCVisibilityNotActiveTest,
                        Values(autosar::UsgModSts1::UsgModAbdnd, autosar::UsgModSts1::UsgModInActv,
                               autosar::UsgModSts1::UsgModCnvinc, autosar::UsgModSts1::UsgModActv));

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_off_if_usg_mode_in_error) {
    vehMode.error(1);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 0};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    btn4_.inject(autosar::PsdNotPsd::Psd);
    btn5_.inject(autosar::PsdNotPsd::Psd);

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
    EXPECT_EQ(255, outvalue.secondsToKeepActive);
}

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_on_when_in_driving_and_Btn4ForUsrSwtPanFrntSts_is_pushed) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    btn4_.inject(autosar::PsdNotPsd::Psd);

    EXPECT_EQ(Vfc::Visibility, outvalue.vfcToActivate);
    EXPECT_EQ(3, outvalue.secondsToKeepActive);
}

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_off_when_in_driving_and_Btn4ForUsrSwtPanFrntSts_is_not_pushed) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    btn4_.inject(autosar::PsdNotPsd::NotPsd);

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
    EXPECT_EQ(255, outvalue.secondsToKeepActive);
}

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_on_when_in_driving_and_Btn5ForUsrSwtPanFrntSts_is_pushed) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    btn5_.inject(autosar::PsdNotPsd::Psd);

    EXPECT_EQ(Vfc::Visibility, outvalue.vfcToActivate);
    EXPECT_EQ(3, outvalue.secondsToKeepActive);
}

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_off_when_in_driving_and_Btn5ForUsrSwtPanFrntSts_is_not_pushed) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    VFCVisibilityLogic vfc;

    btn5_.inject(autosar::PsdNotPsd::NotPsd);

    EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
    EXPECT_EQ(255, outvalue.secondsToKeepActive);
}

TEST_F(VFCVisibilityTest, test_that_vfc_should_be_off_when_signals_in_error) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    // Listen to outgoing dataelement
    DESink<ActivateVfc_info> vfcSink;

    ActivateVfc outvalue{static_cast<Vfc>(-1), 255};
    vfcSink.subscribe([&vfcSink, &outvalue]() { outvalue = vfcSink.get().value(); });

    {
        VFCVisibilityLogic vfc;

        btn4_.error(1);

        EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
        EXPECT_EQ(255, outvalue.secondsToKeepActive);
    }
    {
        VFCVisibilityLogic vfc;

        btn5_.error(1);

        EXPECT_EQ(Vfc::Crash, outvalue.vfcToActivate);
        EXPECT_EQ(255, outvalue.secondsToKeepActive);
    }
}
}
