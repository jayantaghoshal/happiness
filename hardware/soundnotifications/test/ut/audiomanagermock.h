/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManager.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManagerCallback.h>
#include <vendor/delphi/audiomanager/1.0/types.h>

#include <log/log.h>

using namespace vendor::delphi::audiomanager::V1_0;
using namespace ::android::hardware;

class AudioManagerMock : public IAudioManager {  // public vendor::delphi::audiomanager::V1_0::IAudioManager {
  public:
    AudioManagerMock() {}
    virtual ~AudioManagerMock() {}


    MOCK_METHOD3(playSound, Return<void>(int32_t soundType, int32_t soundComp, playSound_cb _aidl_return));
    MOCK_METHOD1(stopSound, Return<AMStatus>(uint32_t connectionId));
    MOCK_METHOD2(setVolume, Return<void>(int32_t sinkId, int32_t volume));
    MOCK_METHOD1(setBass, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setTreble, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setFader, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setSubwoofer, Return<AMStatus>(int32_t step));
    MOCK_METHOD2(setEqualizer, Return<AMStatus>(int32_t band, int32_t step));
    MOCK_METHOD1(setLoudness, Return<AMStatus>(bool step));
    MOCK_METHOD1(setSpeakerDelayPreset, Return<AMStatus>(SpDelayPreset preset));
    MOCK_METHOD1(setBalance, Return<AMStatus>(int32_t step));
    MOCK_METHOD0(getBass, Return<int32_t>());
    MOCK_METHOD0(getTreble, Return<int32_t>());
    MOCK_METHOD0(getBalance, Return<int32_t>());
    MOCK_METHOD0(getFader, Return<int32_t>());
    MOCK_METHOD0(getSubwoofer, Return<int32_t>());
    using getEqualizer_cb = std::function<void(AMStatus status, int32_t equalizerValue)>;
    MOCK_METHOD2(getEqualizer, android::hardware::Return<void>(int32_t band, getEqualizer_cb _hidl_cb));
    MOCK_METHOD0(getLoudness, Return<bool>());
    MOCK_METHOD0(getSpeakerDelayPreset, Return<SpDelayPreset>());

    using defaultVolumeStep_cb = std::function<void(AMStatus status, int32_t volumeStep)>;
    MOCK_METHOD3(defaultVolumeStep,
                 android::hardware::Return<void>(int32_t soundType, int32_t soundComp, defaultVolumeStep_cb _hidl_cb));

    ::android::sp<IAudioManagerCallback> callback_;

    Return<void> subscribe(const ::android::sp<IAudioManagerCallback>& callback) {
        callback_ = callback;
        ALOG(LOG_INFO, "soundwrapperUT.Tests", "AudioManagerMock::subscribe: %p", callback.get());
        return android::hardware::Status::fromStatusT(android::OK);
    }

    Return<void> unsubscribe(const ::android::sp<IAudioManagerCallback>& callback) {
        ALOG(LOG_INFO, "soundwrapperUT.Tests", "AudioManagerMock::unsubscribe: %p", callback.get());
        return android::hardware::Status::fromStatusT(android::OK);
    }
};
