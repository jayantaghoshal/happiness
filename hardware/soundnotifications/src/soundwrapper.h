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

class Sound;
using namespace vendor::delphi::audiomanager::V1_0;

class SoundWrapper : public IAudioManagerCallback {  // Maybe a better name could be found

  public:
    // A "sound" is uniquely identified with SoundID
    struct SoundID {
        AudioTable::SoundType type;
        AudioTable::SoundComponent component;
        bool isValid;  // Used for indicating "No Sound", i.e. an invalid value

        SoundID(AudioTable::SoundType t, AudioTable::SoundComponent c, bool valid = true);
        bool operator<(const SoundID& s) const;  // needed since we use it as a key in an std::map
        SoundID& operator=(SoundID other);       // Copy assignment operator
    };

    // Start playing the given sound id
    // SoundWrapper class is responsible to ensure that sound will be played
    static void play(SoundID soundid);

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    static void stop(SoundID soundid);

    static void registerConnection(Sound* sound, int64_t connectionID);
    static bool isPlaying(SoundID soundid);
    static SoundWrapper* instance();

    bool getInitialized() const;

    bool init(::android::sp<IAudioManager> service);
    void cleanup();

    // reimplemented for IAudioManagerCallback
    virtual ::android::hardware::Return<void> onDisconnected(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onConnected(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onWavFileFinished(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> onRampedIn(uint32_t connectionID) override;
    virtual ::android::hardware::Return<void> ackSetSinkVolumeChange(uint32_t sinkId, int16_t volume) override;

  private:
    SoundWrapper() = default;

    static android::sp<IAudioManager> am_service;

    std::atomic_bool initialized{false};

#ifdef UNIT_TEST
  public:
    static void clearAll();
    static int getSoundState(SoundID);
    static int64_t getConnectionID(SoundID soundid);
    static void setSoundState(SoundID, int state);
#endif
};

/*
 * class to handle each unique sound that shall be played
 * Instances are only created if needed.
 * The class responsible for tracking sound playback status
 * also it has a facility to restart sound if needed
 */

class Sound {
  public:
    // Start playing the given sound id
    void play();

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    void stop();

    Sound(const SoundWrapper::SoundID& soundID, const android::sp<IAudioManager>& service);

    ~Sound() = default;
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
    void onDisconnected();

    void setState(int state);

    SoundWrapper::SoundID getSoundID() const;

  private:
    State _state;
    std::recursive_mutex _stateMutex;

    // connection ID from Audio Manager
    int64_t connectionID;

    // Which soundid we "are"
    SoundWrapper::SoundID _soundID;
    // Sound name from AudioTable
    std::string _name;
    android::sp<IAudioManager> am_service;
    tarmac::eventloop::IDispatcher& restartTimer;
};

}  // namespace SoundNotifications
