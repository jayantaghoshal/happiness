/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "crosstrafficalert.h"
#include "ut_common.h"

#undef LOG_TAG
#define LOG_TAG "CTAUnitTest"

/**
Unit test for REQPROD:243856/MAIN;3	Audio Request for Cross Traffic Alert
*/

class CTATest : public ut_common {};

// helper function to set vehicle mode
void setVehicleMode(DEInjector<autosar::VehModMngtGlbSafe1_info>* vehModMngtGlbSafe1Injector,
                    UsgModSts1 usgModeSts1,
                    CarModSts1 carModSts1) {
    VehModMngtGlbSafe1 drvMode;
    drvMode.UsgModSts = usgModeSts1;
    drvMode.CarModSts1_ = carModSts1;
    vehModMngtGlbSafe1Injector->inject(drvMode);
}

/**
Test Function : CTATest_LeftSoundPlayed
Description:To test the play left CTA warning sound
*/

TEST_F(CTATest, CTATest_LeftSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnLe_info> CtraIndcnLe;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Left)),
                          testing::_))
            .Times(1);

    autosar::CtraIndcnLe_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    CtraIndcnLe.inject(value);
}

/**
Test Function : CTATest_RightSoundPlayed
Description:To test the play right CTA warning sound
*/

TEST_F(CTATest, CTATest_RightSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnRi_info> CtraIndcnRi;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;  // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Right)),
                          testing::_))
            .Times(1);

    autosar::CtraIndcnRi_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    CtraIndcnRi.inject(value);
}

/**
Test Function : CTATest_LeftRightSoundPlayed
Description:To test the play right CTA warning sound
*/

TEST_F(CTATest, CTATest_LeftRightSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnRi_info> CtraIndcnRi;
    DEInjector<autosar::CtraIndcnLe_info> CtraIndcnLe;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;  // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Left)),
                          testing::_))
            .Times(1);

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Right)),
                          testing::_))
            .Times(1);

    autosar::CtraIndcnLe_info::data_elem_type valueL = autosar::CtraIndcn1::CtraWarn;
    autosar::CtraIndcnRi_info::data_elem_type valueR = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    CtraIndcnLe.inject(valueL);
    CtraIndcnRi.inject(valueR);
}

/**
Test Function : CTATest_RightSoundPlayed
Description:To test the play right CTA warning sound
*/

TEST_F(CTATest, CTATest_RightSoundNotPlayed_invalid_vehicle_mode) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnRi_info> CtraIndcnRi;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;  // Setup expectations first

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Right)),
                          testing::_))
            .Times(0);

    autosar::CtraIndcnRi_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);

    CtraIndcnRi.inject(value);
}

/**
Test Function : CTATest_SoundStopped
Description:To test the stop CTA warning as per the requirement.
*/

TEST_F(CTATest, CTATest_SoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnRi_info> CtraIndcnRi;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Right)),
                          testing::_))
            .Times(1);

    autosar::CtraIndcnRi_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);

    CtraIndcnRi.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    value = autosar::CtraIndcn1::NoCtraWarn;
    CtraIndcnRi.inject(value);
}

/** @brief Test Function : CTATest_NoSoundPlayed
 * Description:To test CTA warning sound should not play due to non supported vehicle mode
*/

TEST_F(CTATest, CTATest_NoSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnLe_info> CtraIndcnLe;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Left)),
                          testing::_))
            .Times(0);

    autosar::CtraIndcnLe_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);
    CtraIndcnLe.inject(value);
}

/** @brief Test Function : CTATest_NoSoundPlayed1
 * Description:To test CTA warning sound should not play due to non supported vehicle mode
*/

TEST_F(CTATest, CTATest_NoSoundPlayed1) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnLe_info> CtraIndcnLe;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Left)),
                          testing::_))
            .Times(0);

    autosar::CtraIndcnLe_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModDrvg, CarModSts1::CarModCrash);
    CtraIndcnLe.inject(value);
}

/**
Test Function : CTATest_SoundStoppedOn_VehicleModeChanged
Description:To test the stop ACC Brake release warning as per the requirement.
*/

TEST_F(CTATest, CTATest_SoundStoppedOn_VehicleModeChanged) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnRi_info> CtraIndcnRi;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Right)),
                          testing::_))
            .Times(1);

    autosar::CtraIndcnRi_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);

    CtraIndcnRi.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModCrash);
}

/**
Test Function : CTATest_SoundStoppedOn_VehicleModeChanged1
Description:To test the stop ACC Brake release warning as per the requirement.
*/

TEST_F(CTATest, CTATest_SoundStoppedOn_VehicleModeChanged1) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::CTAWarning cta;

    DEInjector<autosar::CtraIndcnRi_info> CtraIndcnRi;
    DEInjector<autosar::VehModMngtGlbSafe1_info> VehModMngtGlbSafe1;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::CrossTrafficAlert)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::Right)),
                          testing::_))
            .Times(1);

    autosar::CtraIndcnRi_info::data_elem_type value = autosar::CtraIndcn1::CtraWarn;
    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm);

    CtraIndcnRi.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    setVehicleMode(&VehModMngtGlbSafe1, UsgModSts1::UsgModAbdnd, CarModSts1::CarModCrash);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(0);

    value = autosar::CtraIndcn1::NoCtraWarn;
    CtraIndcnRi.inject(value);
}
