/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <AudioTable.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManager.h>
#include "vendor/delphi/audiomanager/1.0/types.h"
//#ifndef UNIT_TEST
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
//#endif
#include <cutils/log.h>

namespace SoundNotifications {

class SoundWrapper
        : public vendor::delphi::audiomanager::V1_0::IAudioManagerCallback {  // Maybe a better name could be found

  public:
    // A "sound" is uniquely identified with SoundID
    struct SoundID {
        const AudioTable::SoundType type;
        const AudioTable::SoundComponent component;

        SoundID(AudioTable::SoundType t, AudioTable::SoundComponent c);
        bool operator<(const SoundID& s) const;  // needed since we use it as a key in an std::map
    };

    // Return result for play() and stop()
    enum class Result {
        OK,
        INVALID_STATE,  // The user of this API called play() or stop() in an invalid state
        PLAY_FAILED     // The underlying API call failed (maybe we dont need two separate result codes??)
    };

    // Start playing the given sound id
    static Result play(SoundID soundid);

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    static Result stop(SoundID soundid);

    static bool isPlaying(SoundID soundid);
    static SoundWrapper* instance();

    ~SoundWrapper();

    bool getInitialized() const;

    bool init(vendor::delphi::audiomanager::V1_0::IAudioManager* service = nullptr);
    void cleanup();

    // reimplemented for IAudioManagerCallback
    virtual ::android::hardware::Return<void> onDisconnected(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onConnected(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onWavFileFinished(uint32_t cId) override;
    virtual ::android::hardware::Return<void> onRampedIn(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> ackSetSinkVolumeChange(uint32_t sinkId, int16_t volume) override;

  private:
    SoundWrapper();

    void onPlayFailed(const AudioTable::SoundType type,
                      const AudioTable::SoundComponent component,
                      const int32_t connectionID,
                      const int32_t error);
    void onPlayBackgroundStarted(const int32_t connectionID) { (void)connectionID; }
    void onPlayBackgroundStopped(const int32_t connectionID, const int32_t reason) {
        (void)connectionID;
        (void)reason;
    }

    static android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> am_service;

    std::atomic_bool initialized{false};

#ifdef UNIT_TEST
  public:
    static void clearAll();
    static int getSoundState(SoundID);
#endif
};
}
