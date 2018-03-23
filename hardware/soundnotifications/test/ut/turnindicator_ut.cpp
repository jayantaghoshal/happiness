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

// DEInjector
#include <ECD_dataelement.h>

#undef LOG_TAG
#define LOG_TAG "turnindicatorUT.Tests"

using namespace SoundNotifications;
using namespace ECDDataElement;
using namespace autosar;

/// Unit test for requirement REQPROD:218373/MAIN;5	Audio request for Turn Indicator

class TurnIndicatorUT : public ::testing::Test {
  public:
    void SetUp() override {
        swrapper = SoundWrapper::instance();
        am_service = ::android::sp<AudioManagerMock>(new AudioManagerMock);
        swrapper->init(am_service);
        SoundWrapper::clearAll();
        DataElementFramework::instance().reset();
        // sut_ =
    }
    void TearDown() override {}
    SoundWrapper* swrapper;
    ::android::sp<AudioManagerMock> am_service;
};

TEST_F(TurnIndicatorUT, LeftOnSignalReceived_leftOnSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());

    TurnIndicator sut_;

    // Set no fault on on the Left front and rear  signals as preconfdition (according to requirements)
    DEInjector<autosar::FltIndcrTurnLeFrnt_info> senderLeFrnt;
    senderLeFrnt.inject(DevErrSts2::NoFlt);
    DEInjector<autosar::FltIndcrTurnLeRe_info> senderLeRear;
    senderLeRear.inject(DevErrSts2::NoFlt);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::TurnIndicator),
                          static_cast<int32_t>(AudioTable::SoundComponent::LeftRight),
                          testing::_))
            .Times(1);

    // Simulate that we receive the FltIndcrTurnLeFrnt signal left on
    DEInjector<autosar::IndcrDisp1WdSts_info> sender;
    sender.inject(IndcrSts1::LeOn);
}
