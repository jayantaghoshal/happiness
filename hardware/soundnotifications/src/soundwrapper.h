/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <AudioTable.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <com/delphi/BnAudioManagerCallback.h>
#include <com/delphi/BpAudioManager.h>
#include <cutils/log.h>

namespace SoundNotifications {

class SoundWrapper : public com::delphi::BnAudioManagerCallback {  // Maybe a better name could be found

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
        PLAY_FAILED     // The underlying cedric::audio API call failed (maybe we dont need two separate result codes??)
    };

    // Start playing the given sound id
    // If continuous==true the sound will be re-triggered as soon as onPlayStopped is received
    // Even if continuous==false the sound may anyhow play forever if the sound is defined as non-flank triggered in the
    // prio matrix.
    static Result play(SoundID soundid, bool continuous = false);

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    static Result stop(SoundID soundid);

    static bool isPlaying(SoundID soundid);

    ~SoundWrapper();

  private:           // Audio callback methods for sound status check
    SoundWrapper();  // shall not be created by anyone except the SoundWrapper::play() method that creates a singleton

    virtual ::android::binder::Status soundStopped(int32_t connectionId) override;

    void onPlayStarted(const AudioTable::SoundType getSoundType,
                       const AudioTable::SoundComponent getSoundComponent,
                       const int32_t connectionID);
    void onPlayStopped(const AudioTable::SoundType getSoundType,
                       const AudioTable::SoundComponent getSoundComponent,
                       const int32_t connectionID,
                       const int32_t getReason);
    void onPlayFailed(const AudioTable::SoundType getSoundType,
                      const AudioTable::SoundComponent getSoundComponent,
                      const int32_t connectionID,
                      const int32_t getErr);
    void onPlayBackgroundStarted(const int32_t connectionID) { (void)connectionID; }
    void onPlayBackgroundStopped(const int32_t connectionID, const int32_t reason) {
        (void)connectionID;
        (void)reason;
    }

    static android::sp<com::delphi::IAudioManager> am_service;
    static android::sp<android::IBinder> binderInterface;

#ifdef UNIT_TEST
  public:
    static void clearAll();
    static int getSoundState(SoundID);
#endif
};
}
