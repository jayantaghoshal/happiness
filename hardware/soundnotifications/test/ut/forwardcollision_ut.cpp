/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "forwardcollision.h"
#include "ut_common.h"

#undef LOG_TAG
#define LOG_TAG "ForwardCollisionUT"

/**
Unit test for REQPROD:229053/MAIN;4	Audible Forward Collision Warning
*/
class ForwardCollisionTest : public ut_common {};

using namespace SoundNotifications;
using namespace ECDDataElement;
using namespace autosar;

using namespace android::hardware;
using namespace testing;

TEST_F(ForwardCollisionTest, ForwardCollision_signal_On_received_sound_played) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;

    // setting the error in the signal initially.
    CllsnFwdWarn_sender.error(2);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);
}

TEST_F(ForwardCollisionTest, Collision_signal_On_received_sound_played) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // setting the error in the signal initially.
    CllsnWarn_sender.error(2);

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnWarn_sender.inject(autosar::OnOff1::On);
}

TEST_F(ForwardCollisionTest, Collision_signal_Off_received_sound_stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnWarn_sender.inject(autosar::OnOff1::Off);
}

TEST_F(ForwardCollisionTest, ForwardCollision_SignalOff_received_sound_stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnFwdWarn_sender.inject(autosar::OnOff1::Off);
}

TEST_F(ForwardCollisionTest, Collision_signal_Error_received_sound_Stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnWarn_sender.error(2);
}

TEST_F(ForwardCollisionTest, FwdCllsn_signal_Error_received_sound_Stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnFwdWarn_sender.error(2);
}

TEST_F(ForwardCollisionTest, FwdCllsn_signal_Err_Cllsn_Off_sound_Stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);
    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnWarn_sender.inject(autosar::OnOff1::Off);
    CllsnFwdWarn_sender.error(2);
}

TEST_F(ForwardCollisionTest, Cllsn_signal_Err_FwdCllsn_Off_sound_Stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);
    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnFwdWarn_sender.inject(autosar::OnOff1::Off);
    CllsnWarn_sender.error(2);
}

TEST_F(ForwardCollisionTest, FwdCllsn_On_Sound_Played_Cllsn_Off_NotStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    // since FwdCllsn signal is still On so not stopped.
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(0);
    CllsnWarn_sender.inject(autosar::OnOff1::Off);
}

TEST_F(ForwardCollisionTest, Cllsn_On_sound_Played_FwdCllsnOff_NotStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    // since Cllsn signal is still On so not stopped.
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(0);
    CllsnFwdWarn_sender.inject(autosar::OnOff1::Off);
}

TEST_F(ForwardCollisionTest, Both_Cllsn_Signal_On_Played_then_Off_sound_Stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);
    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    // Off to one signal will not allow sound to be stopped.
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(0);
    CllsnFwdWarn_sender.inject(autosar::OnOff1::Off);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnWarn_sender.inject(autosar::OnOff1::Off);
}

TEST_F(ForwardCollisionTest, Both_Cllsn_Signal_On_Played_then_Err_sound_Stopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::ForwardCollision fc;
    DEInjector<autosar::CllsnFwdWarnReq_info> CllsnFwdWarn_sender;
    DEInjector<autosar::CllsnWarnReq_info> CllsnWarn_sender;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(
                                  AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity)),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    CllsnFwdWarn_sender.inject(autosar::OnOff1::On);
    CllsnWarn_sender.inject(autosar::OnOff1::On);

    // Setup expectations first
    // Error to one signal will not allow sound to be stopped.
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(0);
    CllsnFwdWarn_sender.error(2);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    CllsnWarn_sender.error(2);
}