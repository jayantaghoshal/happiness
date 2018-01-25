/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManager.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManagerCallback.h>
#include <vendor/delphi/audiomanager/1.0/types.h>

using namespace vendor::delphi::audiomanager::V1_0;
using namespace ::android::hardware;
class AudioManagerMock : public vendor::delphi::audiomanager::V1_0::IAudioManager {
  public:
    AudioManagerMock() {}
    virtual ~AudioManagerMock() {}

    MOCK_METHOD3(playSound, Return<void>(int32_t soundType, int32_t soundComp, playSound_cb _aidl_return));
    MOCK_METHOD1(stopSound, Return<AMStatus>(int64_t connectionId));
    MOCK_METHOD1(subscribe, Return<void>(const ::android::sp<IAudioManagerCallback>& callback));
    MOCK_METHOD2(setVolume, Return<void>(int32_t sinkId, int32_t volume));
    MOCK_METHOD1(setBass, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setTreble, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setFader, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setSubwoofer, Return<AMStatus>(int32_t step));
    MOCK_METHOD2(setEqualizer, Return<AMStatus>(int32_t band, int32_t step));
    MOCK_METHOD1(setLoudness, Return<AMStatus>(int32_t step));
    MOCK_METHOD1(setBalance, Return<AMStatus>(int32_t step));
    MOCK_METHOD0(getBass, Return<int32_t>());
    MOCK_METHOD0(getTreble, Return<int32_t>());
    MOCK_METHOD0(getBalance, Return<int32_t>());
    MOCK_METHOD0(getFader, Return<int32_t>());
    MOCK_METHOD0(getSubwoofer, Return<int32_t>());
    MOCK_METHOD1(getEqualizer, Return<int32_t>(int32_t band));
    MOCK_METHOD0(getLoudness, Return<int32_t>());

    ::android::hardware::Return<void> unsubscribe(const ::android::sp<IAudioManagerCallback>& callback) override;
};
