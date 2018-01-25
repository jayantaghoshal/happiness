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

#undef LOG_TAG
#define LOG_TAG "soundwrapperUT.Tests"

// implement missed references
// vendor::delphi::audiomanager::V1_0::IAudioManager::~IAudioManager() {}
// vendor::delphi::audiomanager::V1_0::IAudioManager::IAudioManager() {}
// const android::String16 &com::delphi::IAudioManager::getInterfaceDescriptor() const {
//    static android::String16 s("");
//    return s;
//}

// const ::android::String16 com::delphi::IAudioManagerCallback::descriptor;
// com::delphi::IAudioManagerCallback::~IAudioManagerCallback() {}
// com::delphi::IAudioManagerCallback::IAudioManagerCallback() {}
// const android::String16 &com::delphi::IAudioManagerCallback::getInterfaceDescriptor() const {
//    static android::String16 s("");
//    return s;
//}

//::android::sp<com::delphi::IAudioManager> com::delphi::IAudioManager::asInterface(
//        android::sp<android::IBinder> const &) {
//    return ::android::sp<com::delphi::IAudioManager>();
//}

using namespace SoundNotifications;
::android::hardware::Return<void> AudioManagerMock::unsubscribe(const ::android::sp<IAudioManagerCallback>& callback) {
    (void)callback;
    return android::hardware::Return<void>();
}

class SoundWrapperUT : public ::testing::Test {
  public:
    void SetUp() override {
        swrapper = SoundWrapper::instance();
        am_service = new AudioManagerMock;
        swrapper->init(am_service);
        //        SoundWrapper::clearAll();
    }
    void TearDown() override { delete am_service; }
    SoundWrapper* swrapper;
    AudioManagerMock* am_service;
};

TEST_F(SoundWrapperUT, playSound_correctSoundPlayed_SoundStopsByItself) {
    ALOGI("Starting %s", test_info_->name());

    android::hardware::Return<void> ret;

    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::TurnIndicator),
                          static_cast<int32_t>(AudioTable::SoundComponent::LeftRight),
                          testing::_))
            .Times(1);
    //.WillOnce(testing::Return(ret));

    // DO the call to soundwrapper
    auto result = SoundWrapper::play(
            SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator, AudioTable::SoundComponent::LeftRight));

    EXPECT_EQ(SoundWrapper::Result::OK, result);
    // SImulate callback

    // Idle->Starting->Playing->Idle

    /*    auto soundToPlay = SoundWrapper::SoundID(SoundType::AndroidPhone, SoundComponent::Left);
        auto result      = SoundWrapper::play(soundToPlay);
        EXPECT_EQ(SoundWrapper::Result::OK, result);
        EXPECT_EQ(soundToPlay.type, playedSoundType);
        EXPECT_EQ(soundToPlay.component, playedSoundComponent);
        EXPECT_TRUE(listener != nullptr);

        // Directly after play we shall be in state Starting
        EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

        // Advance state with onPlayStarted to take us to state Playing
        listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
        EXPECT_EQ(PlayState::Playing, SoundWrapper::getSoundState(soundToPlay));

        // Advance state with onPlayStopped to take us back to Idle
        listener->onPlayStopped(soundToPlay.type, soundToPlay.component, 88, PlayStoppedReason::Finished);
        EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
        */
}

/*
TEST_F(SoundWrapperUT, playSound_correctSoundPlayed_SoundStopsByStopSound)
{
    // Idle->Starting->Playing->Stopping->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::HoodOpen, SoundComponent::Right);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayStarted to take us to Playing
    listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
    EXPECT_EQ(PlayState::Playing, SoundWrapper::getSoundState(soundToPlay));

    // Call stopSound() t go to state Stopping
    result = SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, stoppedSoundType);
    EXPECT_EQ(soundToPlay.component, stoppedSoundComponent);
    EXPECT_EQ(PlayState::Stopping, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayStopped to take us back to Idle
    listener->onPlayStopped(soundToPlay.type, soundToPlay.component, 88, PlayStoppedReason::Finished);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, playSoundStopDirectly_correctSoundPlayedAndStates)
{
    // Idle->Starting->Stopping->Stopping->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::HoodOpen, SoundComponent::Right);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Stop before coming to Playing takes us to Stopping
    result = SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(PlayState::Stopping, SoundWrapper::getSoundState(soundToPlay));

    // When we get onPlayStarted we will stop the sound but stay in state
    listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
    EXPECT_EQ(soundToPlay.type, stoppedSoundType);
    EXPECT_EQ(soundToPlay.component, stoppedSoundComponent);
    EXPECT_EQ(PlayState::Stopping, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayStopped to take us back to Idle
    listener->onPlayStopped(soundToPlay.type, soundToPlay.component, 88, PlayStoppedReason::Finished);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, playWhenInStopping_newStateStarting)
{
    // Idle->Starting->Playing->Stopping->Starting->Starting

    auto soundToPlay = SoundWrapper::SoundID(SoundType::TurnIndicator, SoundComponent::LeftRightBroken);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayStarted to take us to Playing
    listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
    EXPECT_EQ(PlayState::Playing, SoundWrapper::getSoundState(soundToPlay));

    // Call stopSound() to go to state Stopping
    result = SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, stoppedSoundType);
    EXPECT_EQ(soundToPlay.component, stoppedSoundComponent);
    EXPECT_EQ(PlayState::Stopping, SoundWrapper::getSoundState(soundToPlay));

    // Play again and we shall come to Starting
    result = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // If we get onPlayStopped here we shall START the sound again and stay in Starting
    playedSoundType      = (SoundType)0xFFFF;
    playedSoundComponent = (SoundComponent)0xFFFF;
    listener->onPlayStopped(soundToPlay.type, soundToPlay.component, 88, PlayStoppedReason::Finished);
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, onPlayFailedFromStarting_alwaysBackToIdle)
{
    // Idle->Starting->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::AndroidPhone, SoundComponent::Left);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayFailed to take us to state Idle
    listener->onPlayFailed(soundToPlay.type, soundToPlay.component, 99, PlayError::Unknown);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, onPlayFailedFromPlaying_alwaysBackToIdle)
{
    // Idle->Starting->Playing->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::SSDoorOpenVehicleActive, SoundComponent::Left);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Advance state with onPlayStarted to take us to Playing
    listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
    EXPECT_EQ(PlayState::Playing, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayFailed to take us to state Idle
    listener->onPlayFailed(soundToPlay.type, soundToPlay.component, 99, PlayError::Unknown);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, onPlayFailedFromStopping_alwaysBackToIdle)
{
    // Idle->Starting->Playing->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::SSDoorOpenVehicleActive, SoundComponent::Left);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(PlayState::Stopping, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayFailed to take us to state Idle
    listener->onPlayFailed(soundToPlay.type, soundToPlay.component, 99, PlayError::Unknown);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, playSoundFailsInIdle_StayInIdle)
{
    // Idle->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::AndroidPhone, SoundComponent::Left);
    playSoundResult  = false;
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::CEDRIC_FAILED, result);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, playSoundFailsInStarting_ToIdle)
{
    // Idle->Starting->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::AndroidPhone, SoundComponent::Left);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayStopped. playSound returns false -> we go to Idle
    playSoundResult = false;
    listener->onPlayStopped(soundToPlay.type, soundToPlay.component, 77, PlayStoppedReason::Finished);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, stopSoundFailsInStopping_toIdle)
{
    // Idle->Starting->Stopping->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::HoodOpen, SoundComponent::Right);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Stop before coming to Playing takes us to Stopping
    result = SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(PlayState::Stopping, SoundWrapper::getSoundState(soundToPlay));

    // When we get onPlayStarted we will stop the sound but it fails -> to Idle
    stopSoundResult = false;
    listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}

TEST_F(SoundWrapperUT, stopSoundFailsInPlaying_toIdle)
{
    // Idle->Starting->Playing->Idle

    auto soundToPlay = SoundWrapper::SoundID(SoundType::HoodOpen, SoundComponent::Right);
    auto result      = SoundWrapper::play(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::OK, result);
    EXPECT_EQ(soundToPlay.type, playedSoundType);
    EXPECT_EQ(soundToPlay.component, playedSoundComponent);
    EXPECT_TRUE(listener != nullptr);

    // Directly after play we shall be in state Starting
    EXPECT_EQ(PlayState::Starting, SoundWrapper::getSoundState(soundToPlay));

    // Advance state with onPlayStarted to take us to Playing
    listener->onPlayStarted(soundToPlay.type, soundToPlay.component, 77);
    EXPECT_EQ(PlayState::Playing, SoundWrapper::getSoundState(soundToPlay));

    // Call stopSound() but it fails -> goto Idle
    stopSoundResult = false;
    result          = SoundWrapper::stop(soundToPlay);
    EXPECT_EQ(SoundWrapper::Result::CEDRIC_FAILED, result);
    EXPECT_EQ(PlayState::Idle, SoundWrapper::getSoundState(soundToPlay));
}
*/
