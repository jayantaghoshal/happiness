/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <soundwrapper.h>
#include <cinttypes>
#include <iostream>
#include "audiomanagermock.h"

#include "ut_common.h"

#undef LOG_TAG
#define LOG_TAG "soundwrapperUT.Tests"

using namespace SoundNotifications;
using namespace AudioTable;

class SoundWrapperUT : public ut_common {};

using namespace android::hardware;

using namespace testing;

TEST_F(SoundWrapperUT, playSound_correctSoundPlayed_SoundStopsByItself) {
    ALOGI("Starting %s", test_info_->name());

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::TurnIndicator)),
                          static_cast<int32_t>(SoundComponent::LeftRight),
                          testing::_))
            .Times(1);

    auto soundToPlay = SoundWrapper::SoundID(SoundType::TurnIndicator, SoundComponent::LeftRight);
    // DO the call to soundwrapper
    SoundWrapper::play(soundToPlay);
    // Directly after play we shall be in state Starting
    EXPECT_TRUE(SoundWrapper::isPlaying(soundToPlay));
    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(SoundWrapperUT, playSound_correctSoundPlayed_SoundStopsByStopSound) {
    ALOGI("Starting %s", test_info_->name());
    // Idle->Starting->Playing->Stopping->Idle

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::TurnIndicator)),
                          static_cast<int32_t>(SoundComponent::LeftRight),
                          testing::_))
            .Times(1);

    auto soundToPlay = SoundWrapper::SoundID(SoundType::TurnIndicator, SoundComponent::LeftRight);
    // DO the call to soundwrapper
    SoundWrapper::play(soundToPlay);

    EXPECT_TRUE(SoundWrapper::isPlaying(soundToPlay));

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    SoundWrapper::stop(soundToPlay);

    EXPECT_FALSE(SoundWrapper::isPlaying(soundToPlay));

    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(SoundWrapperUT, playSoundStopDirectly_correctSoundPlayedAndStates) {
    ALOGI("Starting %s", test_info_->name());
    // Idle->Starting->Stopping->Stopping->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::TurnIndicator, SoundComponent::LeftRight);

    EXPECT_EQ(-1, SoundWrapper::getSoundState(soundToPlay));

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::TurnIndicator)),
                          static_cast<int32_t>(SoundComponent::LeftRight),
                          testing::_))
            .Times(1);

    SoundWrapper::play(soundToPlay);
    // Directly after play we shall be in state Starting
    EXPECT_EQ(static_cast<int>(Sound::State::Playing), SoundWrapper::getSoundState(soundToPlay));

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    // Stop before coming to Playing takes us to Stopping
    SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(SoundWrapperUT, playWhenInStopping_newStateStarting) {
    ALOGI("Starting %s", test_info_->name());
    // Idle->Starting->Playing->Stopping->Starting->Starting
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::TurnIndicator)),
                          static_cast<int32_t>(SoundComponent::LeftRightBroken),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::TurnIndicator, SoundComponent::LeftRightBroken);
    SoundWrapper::play(soundToPlay);

    // Advance state with onPlayStarted to take us to Playing
    EXPECT_EQ(static_cast<int>(Sound::State::Playing), SoundWrapper::getSoundState(soundToPlay));

    // Call stopSound() to go to state Stopping
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));

    // Play again and we shall come to Playing
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::TurnIndicator)),
                          static_cast<int32_t>(SoundComponent::LeftRightBroken),
                          testing::_))
            .Times(1);
    SoundWrapper::play(soundToPlay);
    EXPECT_EQ(static_cast<int>(Sound::State::Playing), SoundWrapper::getSoundState(soundToPlay));
    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(SoundWrapperUT, onPlayFailedFromStarting_alwaysBackToIdle) {
    // Idle->Starting->Idle

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::AndroidPhone)),
                          static_cast<int32_t>(SoundComponent::Left),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::AndroidPhone, SoundComponent::Left);
    SoundWrapper::play(soundToPlay);

    // Advance state with on play failed to take us to state Idle
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, onPlayFailedFromPlaying_alwaysBackToIdle) {
    // Idle->Starting->Playing->Idle
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::SSDoorOpenVehicleActive)),
                          static_cast<int32_t>(SoundComponent::Left),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::SSDoorOpenVehicleActive, SoundComponent::Left);
    SoundWrapper::play(soundToPlay);
    // Advance state with onPlayFailed to take us to state Idle
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, onPlayFailedFromStopping_alwaysBackToIdle) {
    // Idle->Starting->Playing->Idle
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::SSDoorOpenVehicleActive)),
                          static_cast<int32_t>(SoundComponent::Left),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::SSDoorOpenVehicleActive, SoundComponent::Left);
    SoundWrapper::play(soundToPlay);

    SoundWrapper::stop(soundToPlay);

    // Advance state with onPlayFailed to take us to state Idle
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, playSoundFailsInIdle_StayInIdle) {
    // Idle->Idle
    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::AndroidPhone)),
                          static_cast<int32_t>(SoundComponent::Left),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::AndroidPhone, SoundComponent::Left);
    SoundWrapper::play(soundToPlay);
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, stopSoundFailsInStopping_toIdle) {
    // Idle->Starting->Stopping->Idle

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::BeltReminder)),
                          static_cast<int32_t>(SoundComponent::Right),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::BeltReminder, SoundComponent::Right);
    SoundWrapper::play(soundToPlay);

    // Idle because of invalid sound
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));

    SoundWrapper::stop(soundToPlay);
    // still Idle
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));

    // When we get onPlayStarted we will stop the sound but it fails -> to Idle
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, stopSoundFailsInPlaying_toIdle) {
    // Idle->Starting->Playing->Idle

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::BeltReminder)),
                          static_cast<int32_t>(SoundComponent::Warn1),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::BeltReminder, SoundComponent::Warn1);
    SoundWrapper::play(soundToPlay);

    // Advance state with onPlayStarted to take us to Playing
    EXPECT_EQ(static_cast<int>(Sound::State::Playing), SoundWrapper::getSoundState(soundToPlay));

    // Expecting to call stopSound() twice
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, playSound_stopSound_withoutOnDisconnected_playAgain) {
    // Idle->Starting->Playing->Idle

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::BeltReminder)),
                          static_cast<int32_t>(SoundComponent::Warn1),
                          testing::_))
            .Times(1);
    auto soundToPlay = SoundWrapper::SoundID(SoundType::BeltReminder, SoundComponent::Warn1);
    SoundWrapper::play(soundToPlay);

    // Advance state with onPlayStarted to take us to Playing
    EXPECT_EQ(static_cast<int>(Sound::State::Playing), SoundWrapper::getSoundState(soundToPlay));

    // Expecting to call stopSound() twice
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    callOnDisconnect = false;
    SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(static_cast<int>(Sound::State::Stopping), SoundWrapper::getSoundState(soundToPlay));

    EXPECT_CALL(*am_service,
                playSound(hidl_string(AudioTable::getSoundTypeName(SoundType::BeltReminder)),
                          static_cast<int32_t>(SoundComponent::Warn1),
                          testing::_))
            .Times(0);
    SoundWrapper::play(soundToPlay);

    // Expecting the state to be Stopping because onDisconnected has not called yet.
    EXPECT_EQ(static_cast<int>(Sound::State::Stopping), SoundWrapper::getSoundState(soundToPlay));

    swrapper->onDisconnected(SoundWrapper::getConnectionID(soundToPlay));
    // Expecting the state to be playing because onDisconnected has not called yet.
    EXPECT_EQ(static_cast<int>(Sound::State::Idle), SoundWrapper::getSoundState(soundToPlay));
}