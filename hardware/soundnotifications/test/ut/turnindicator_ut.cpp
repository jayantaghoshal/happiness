/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include <cutils/log.h>

#include <soundwrapper.h>
#include "audiomanagermock.h"
#include "turnindicator.h"
#include "ut_common.h"

// DEInjector
#include <ECD_dataelement.h>

#undef LOG_TAG
#define LOG_TAG "turnindicatorUT.Tests"

using namespace SoundNotifications;
using namespace ECDDataElement;
using namespace autosar;

using namespace android::hardware;
using namespace testing;

/// Unit test for requirement REQPROD:218373/MAIN;5	Audio request for Turn Indicator

class TurnIndicatorUT : public ut_common {};

TEST_F(TurnIndicatorUT, LeftOnSignalReceived_leftTurnIndSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    // Set no fault on on the Left front and rear signals as preconfdition (according to requirements)
    DEInjector<autosar::FltIndcrTurnLeFrnt_info> senderLeFrnt;
    senderLeFrnt.inject(DevErrSts2::NoFlt);
    DEInjector<autosar::FltIndcrTurnLeRe_info> senderLeRear;
    senderLeRear.inject(DevErrSts2::NoFlt);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::TurnIndicator)),
                          static_cast<int32_t>(AudioTable::SoundComponent::LeftRight),  // Same sound for left and right
                          testing::_))
            .Times(1);

    // Simulate that we receive IndcrSts1 signal left on
    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::LeOn);
}

TEST_F(TurnIndicatorUT, RightOnSignalReceived_rightTurnIndSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    // Set no fault on on the Right front and rear signals as preconfdition (according to requirements)
    DEInjector<autosar::FltIndcrTurnRiFrnt_info> senderRiFrnt;
    senderRiFrnt.inject(DevErrSts2::NoFlt);
    DEInjector<autosar::FltIndcrTurnRiRe_info> senderRiRear;
    senderRiRear.inject(DevErrSts2::NoFlt);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::TurnIndicator)),
                          static_cast<int32_t>(AudioTable::SoundComponent::LeftRight),  // Same sound for left and right
                          testing::_))
            .Times(1);

    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::RiOn);
}

TEST_F(TurnIndicatorUT, LeftAndRightOnSignalReceived_hazardSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::TurnIndicator)),
                          static_cast<int32_t>(AudioTable::SoundComponent::Hazard),  // Same sound for left and right
                          testing::_))
            .Times(1);

    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::LeAndRiOn);
}

TEST_F(TurnIndicatorUT, LeftOnSignalReceivedLeftRearFaulty_leftTurnIndSoundBroken) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    // Set no fault on on the Right front and rear signals as preconfdition (according to requirements)
    DEInjector<autosar::FltIndcrTurnLeFrnt_info> senderLeFrnt;
    senderLeFrnt.inject(DevErrSts2::NoFlt);
    DEInjector<autosar::FltIndcrTurnLeRe_info> senderLeRear;
    senderLeRear.inject(DevErrSts2::Flt);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(AudioTable::SoundType::TurnIndicator)),
                          static_cast<int32_t>(
                                  AudioTable::SoundComponent::LeftRightBroken),  // Same sound for left and right
                          testing::_))
            .Times(1);

    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::LeOn);
}

TEST_F(TurnIndicatorUT, RightOnSignalReceivedRightFrontSignalInErrorState_noSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    DEInjector<autosar::FltIndcrTurnRiFrnt_info> senderRiFrnt;
    senderRiFrnt.error(2);
    DEInjector<autosar::FltIndcrTurnRiRe_info> senderRiRear;
    senderRiRear.inject(DevErrSts2::NoFlt);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(testing::_,
                          testing::_,
                          testing::_))
            .Times(0);  // expect no call to audio manager

    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::RiOn);
}

TEST_F(TurnIndicatorUT, LeftOnSignalReceivedRightFrontSignalInErrorState_noSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    DEInjector<autosar::FltIndcrTurnLeFrnt_info> senderLeFrnt;
    senderLeFrnt.error(2);
    DEInjector<autosar::FltIndcrTurnLeRe_info> senderLeRear;
    senderLeRear.inject(DevErrSts2::NoFlt);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(testing::_,
                          testing::_,
                          testing::_))
            .Times(0);  // expect no call to audio manager

    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::LeOn);
}

TEST_F(TurnIndicatorUT, IndicatorStatusToOff_stopSoundCalled) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    DEInjector<autosar::FltIndcrTurnLeFrnt_info> senderLeFrnt;
    senderLeFrnt.inject(DevErrSts2::NoFlt);
    DEInjector<autosar::FltIndcrTurnLeRe_info> senderLeRear;
    senderLeRear.inject(DevErrSts2::NoFlt);

    // Setup expectations first
    EXPECT_CALL(*am_service, playSound(testing::_, testing::_, testing::_)).Times(AtLeast(1));
    // Simulate that we receive IndcrSts1 left on, should trigger sound play
    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::LeOn);

    // Simulate that we receive IndcrSts1 off, should trigger sound stop
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(AtLeast(1));
    sender.inject(IndcrSts1::Off);
}