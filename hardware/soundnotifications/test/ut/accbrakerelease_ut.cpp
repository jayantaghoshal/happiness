/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "accbrakerelease.h"
#include "ut_common.h"

#undef LOG_TAG
#define LOG_TAG "AccBrakeReleaseUnitTest"

/**
Unit test for REQPROD:446629/MAIN;0 Sound request for safety belt reminder
*/

class AccBrakeReleaseTest : public ut_common {};

/**
Test Function : AccBrakeRelease_SoundPlayed
Description: To test the audio sound play for the ACC Brake release
*/
TEST_F(AccBrakeReleaseTest, AccBrakeReleaseTest_SoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ACCBrakeReleaseWarning br;

    DEInjector<autosar::BrkRelsWarnReq_info> brkRelWarnSnd;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::ACCBrakeReleaseWarning)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::NotAvailable)),
                          testing::_))
            .Times(1);

    autosar::BrkRelsWarnReq_info::data_elem_type value = autosar::NoYes1::Yes;
    brkRelWarnSnd.inject(value);
}

/**
Test Function : AccBrakeRelease_SoundStopped
Description:To test the stop ACC Brake release warning as per the requirement.
*/

TEST_F(AccBrakeReleaseTest, AccBrakeRelease_SoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ACCBrakeReleaseWarning br;

    DEInjector<autosar::BrkRelsWarnReq_info> brkRelWarnSnd;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::ACCBrakeReleaseWarning)),
                          hidl_string(AudioTable::getSoundComponentName(AudioTable::SoundComponent::NotAvailable)),
                          testing::_))
            .Times(1);

    autosar::BrkRelsWarnReq_info::data_elem_type value = autosar::NoYes1::Yes;
    brkRelWarnSnd.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    value = autosar::NoYes1::No;
    brkRelWarnSnd.inject(value);
}
