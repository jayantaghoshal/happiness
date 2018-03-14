/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <AudioTable.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <cutils/log.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManager.h>
#include <cstdint>
#include "vendor/delphi/audiomanager/1.0/types.h"
// restart sound facility
#include "IDispatcher.h"

namespace SoundNotifications {

class SoundWrapper {  // Maybe a better name could be found

  public:
    // A "sound" is uniquely identified with SoundID
    struct SoundID {
        const AudioTable::SoundType type;
        const AudioTable::SoundComponent component;

        SoundID(AudioTable::SoundType t, AudioTable::SoundComponent c);
        bool operator<(const SoundID& s) const;  // needed since we use it as a key in an std::map
    };

    // Start playing the given sound id
    // SoundWrapper class is responsible to ensure that sound will be played
    static void play(SoundID soundid);

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    static void stop(SoundID soundid);

    static bool isPlaying(SoundID soundid);
    static SoundWrapper* instance();

    virtual ~SoundWrapper();

    bool getInitialized() const;

    bool init(vendor::delphi::audiomanager::V1_0::IAudioManager* service = nullptr);
    void cleanup();

  private:
    SoundWrapper();

    static android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> am_service;

    std::atomic_bool initialized{false};

#ifdef UNIT_TEST
  public:
    static void clearAll();
    static int getSoundState(SoundID);
#endif
};

/*
 * class to handle each unique sound that shall be played
 * Instances are only created if needed.
 * The class responsible for tracking sound playback status
 * also it has a facility to restart sound if needed
 */

class Sound : public vendor::delphi::audiomanager::V1_0::IAudioManagerCallback {
  public:
    // Start playing the given sound id
    void play();

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    void stop();

    Sound(const SoundWrapper::SoundID& soundID,
          const android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>& service);

    virtual ~Sound() override;
    // play callbacks but only for this sound
    void onPlayStarted();
    void onPlayStopped(int32_t reason);

    std::string name() const;

    // Play state
    enum class State { Idle, Starting, Playing, Stopping };

    State getState() { return _state; }

    int64_t getConnectionID() const;

    // Timer callback
    void onTimeout();

    // reimplemented for IAudioManagerCallback
    virtual ::android::hardware::Return<void> onDisconnected(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onConnected(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onWavFileFinished(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onRampedIn(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> ackSetSinkVolumeChange(uint32_t sinkId, int16_t volume) override;

  private:
    State _state;
    std::recursive_mutex _stateMutex;

    // connection ID from Audio Manager
    int64_t connectionID;

    // Which soundid we "are"
    SoundWrapper::SoundID _soundID;
    // Sound name from AudioTable
    const std::string _name;
    android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> am_service;
    tarmac::eventloop::IDispatcher& restartTimer;
};

}  // namespace
