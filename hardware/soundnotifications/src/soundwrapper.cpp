/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "soundwrapper.h"
#include <IDispatcher.h>
#include <memory>
#include <mutex>

#undef LOG_TAG
#define LOG_TAG "SoundNotificationWrp"

using namespace tarmac::eventloop;

namespace SoundNotifications {

android::sp<com::delphi::IAudioManager> SoundWrapper::am_service;
android::sp<android::IBinder> SoundWrapper::binderInterface;

/*
 * class to handle each unique sound that shall be played
 * Instances are only created if needed.
 */
class Sound {
  public:
    // Start playing the given sound id
    SoundWrapper::Result play(bool continuous);

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    SoundWrapper::Result stop();

    Sound(SoundWrapper::SoundID, android::sp<com::delphi::IAudioManager> service);

    // play callbacks but only for this sound
    void onPlayStarted();
    void onPlayStopped(int32_t reason);
    void onPlayFailed(int32_t error);

    // Timer callback
    void onTimeout();

    std::string name() const;

    // Play state
    enum class State {
        Idle,
        Starting,
        Playing,
        Stopping,
        ShallPlay  // this is an intermediate state where we continuously tries to get our sound to play
    };

    State getState() { return _state; }

  private:
    void startTimer();
    void startTimerNow();
    void stopTimer();

    State _state;
    std::recursive_mutex _stateMutex;

    // Which soundid we "are"
    int64_t connectionID;

    SoundWrapper::SoundID _soundID;
    const std::string _name;
    bool _continuous;
    IDispatcher::JobId _timerJobID;
    android::sp<com::delphi::IAudioManager> am_service;
};

// ==========================================================
// implementation Sound
Sound::Sound(SoundWrapper::SoundID soundID, android::sp<com::delphi::IAudioManager> service)
    : _state(State::Idle),
      _soundID(soundID),
      _name(AudioTable::getSourceName(soundID.type, soundID.component)),
      _continuous(false),
      _timerJobID(0),
      am_service(service) {}

SoundWrapper::Result Sound::play(bool continuous) {
    _continuous = continuous;

    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);

    switch (_state) {
        case State::Idle: {
            _state = State::Starting;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            android::binder::Status status = am_service->playSound(
                    static_cast<int32_t>(_soundID.type), static_cast<int32_t>(_soundID.component), &connectionID);

            if (status.isOk()) {
                ALOGV("Sound::play Idle->Starting %s", _name.c_str());
                _state = State::Idle;
                return SoundWrapper::Result::OK;
            } else {
                if (!_continuous) {
                    ALOGW("Sound::play failed to playSound %s. Error: %s",
                          _name.c_str(),
                          status.exceptionMessage().string());
                    _state = State::Idle;  // stay in Idle
                    return SoundWrapper::Result::PLAY_FAILED;
                } else {
                    ALOGV("Sound::play Idle->ShallPlay %s", _name.c_str());
                    _state = State::ShallPlay;
                    startTimer();
                    return SoundWrapper::Result::OK;
                }
            }
            break;
        }
        case State::Playing:
            ALOGD("Sound::play invalid state Playing %s", _name.c_str());
            return SoundWrapper::Result::INVALID_STATE;
            break;

        case State::Starting:
            ALOGD("Sound::play invalid state Starting %s", _name.c_str());
            return SoundWrapper::Result::INVALID_STATE;
            break;

        case State::Stopping:
            _state = State::Starting;
            ALOGV("Sound::play Stopping->Starting %s", _name.c_str());
            return SoundWrapper::Result::OK;
            break;

        case State::ShallPlay:
            ALOGD("Sound::play invalid state ShallPlay %s", _name.c_str());
            return SoundWrapper::Result::INVALID_STATE;
            break;

        default:
            ALOGW("Sound::play Invalid state %d %s", (int)_state, _name.c_str());
            return SoundWrapper::Result::INVALID_STATE;
            break;
    }
    return SoundWrapper::Result::INVALID_STATE;
}

SoundWrapper::Result Sound::stop() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            ALOGD("Sound::stop invalid state Idle %s", _name.c_str());
            return SoundWrapper::Result::INVALID_STATE;
            break;

        case State::Playing: {
            _state = State::Stopping;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            ::android::binder::Status status = am_service->stopSound(connectionID);
            if (status.isOk()) {
                ALOGV("Sound::stop Playing->Stopping %s", _name.c_str());
                return SoundWrapper::Result::OK;
            } else {
                ALOGE("Sound::stop failed to stopSound %s, jumping to Idle %s",
                      status.exceptionMessage().string(),
                      _name.c_str());
                _state = State::Idle;  // got to Idle
                return SoundWrapper::Result::PLAY_FAILED;
            }
            break;
        }

        case State::Starting:
            _state = State::Stopping;
            ALOGV("Sound::stop Starting->Stopping %s", _name.c_str());
            return SoundWrapper::Result::OK;
            break;

        case State::Stopping:
            ALOGV("Sound::stop invalid state Stopping %s", _name.c_str());
            return SoundWrapper::Result::INVALID_STATE;
            break;

        case State::ShallPlay:
            _state = State::Idle;
            stopTimer();
            ALOGV("Sound::stop ShallPlay->Idle %s", _name.c_str());
            return SoundWrapper::Result::OK;
    }
    return SoundWrapper::Result::INVALID_STATE;
}

std::string Sound::name() const { return _name; }

void Sound::onPlayStarted() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state Idle %s", _name.c_str());
            break;

        case State::Playing:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state Playing %s", _name.c_str());
            break;

        case State::Starting:
            _state = State::Playing;
            ALOGV("Sound::onPlayStarted Starting->Playing %s", _name.c_str());
            break;

        case State::Stopping:
            if (stop() != SoundWrapper::Result::OK) {
                _state = State::Idle;
                ALOGE("Sound::onPlayStarted failed to stopSound %s", _name.c_str());
            } else {
                ALOGV("Sound::onPlayStarted Stopping->Stopping %s", _name.c_str());
            }
            break;

        case State::ShallPlay:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state ShallPlay %s", _name.c_str());
            break;

        default:
            ALOGE("Sound::onPlayStarted Invalid state %d %s", (int)_state, _name.c_str());
            break;
    }
}

void Sound::onPlayStopped(int32_t reason) {
    ALOGD("Sound::onPlayStopped, reason %d %s", (int)reason, _name.c_str());
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            // Do nothing
            ALOGD("Sound::onPlayStopped invalid state Idle %s", _name.c_str());
            break;

        case State::Playing:
            if (!_continuous) {
                _state = State::Idle;
                ALOGV("Sound::onPlayStopped Playing->Idle %s", _name.c_str());
            } else {
                _state = State::ShallPlay;
                startTimerNow();
                ALOGV("Sound::onPlayStopped Playing->ShallPlay %s", _name.c_str());
            }
            break;

        case State::Starting:
            if (play(_continuous) != SoundWrapper::Result::OK) {
                if (!_continuous) {
                    _state = State::Idle;
                    ALOGE("Sound::onPlayStopped failed to playSound %s", _name.c_str());
                } else {
                    _state = State::ShallPlay;
                    startTimer();
                    ALOGV("Sound::onPlayStopped Starting->ShallPlay %s", _name.c_str());
                }
            } else {
                ALOGV("Sound::onPlayStopped Starting->Starting %s", _name.c_str());
            }
            break;

        case State::Stopping:
            _state = State::Idle;
            ALOGV("Sound::onPlayStopped Stopping->Idle %s", _name.c_str());
            break;

        case State::ShallPlay:
            // Do nothing
            ALOGV("Sound::onPlayStopped invalid state ShallPlay %s", _name.c_str());
            break;

        default:
            ALOGE("Sound::onPlayStopped Invalid state %d %s", (int)_state, _name.c_str());
            break;
    }
}

void Sound::onPlayFailed(int32_t error) {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::ShallPlay:
            // Do nothing
            ALOGD("Sound::onPlayFailed invalid state ShallPlay %s %i", _name.c_str(), (int)error);
            break;

        default:
            _state = State::Idle;
            ALOGD("Sound::onPlayFailed, error %i %i ->Idle %s", (int)error, (int)_state, _name.c_str());
            break;
    }
}

void Sound::onTimeout() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::ShallPlay:
            _state = State::Starting;
            if (play(_continuous) == SoundWrapper::Result::OK) {
                ALOGV("Sound::onTimeout ShallPlay->Starting %s", _name.c_str());
            } else {
                _state = State::ShallPlay;
                startTimer();
                ALOGV("Sound::play ShallPlay->ShallPlay, restarting timer %s", _name.c_str());
            }
            break;

        default:
            _state = State::Idle;
            ALOGD("Sound::onTimeout %i ->Idle %s", (int)_state, _name.c_str());
            break;
    }
}

void Sound::startTimer() {
    _timerJobID = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay((std::chrono::milliseconds)200,
                                                                       [this]() { onTimeout(); });
}

void Sound::startTimerNow() {
    IDispatcher::GetDefaultDispatcher().Enqueue([this]() { onTimeout(); });
}

void Sound::stopTimer() {
    if (_timerJobID != 0) {
        IDispatcher::GetDefaultDispatcher().Cancel(_timerJobID);
        _timerJobID = 0;
    }
}

// ==========================================================
// implementation SoundWrapper

// Only used by the SoundWrapper class
static std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>> sounds;
static std::recursive_mutex soundsMutex;

SoundWrapper::SoundWrapper() {
    using namespace android;
    sp<IServiceManager> sm = defaultServiceManager();

    if (sm.get() == nullptr) {
        ALOGW("service manager is null");
        return;
    }

    ALOGI("%s: Starting getService(am_service)", __FUNCTION__);
    int retries = 3;
    while (binderInterface.get() == nullptr && --retries >= 0) {
        binderInterface = sm->getService(String16("am_service"));

        if (binderInterface.get() == nullptr) {
            ALOGW("%s: getService(am_service) failed", __FUNCTION__);
            usleep(500000);
        }
    }
    ALOGI("%s: getService(am_service) %p", __FUNCTION__, binderInterface.get());
    if (binderInterface.get() == nullptr) {
        ALOGW("%s: am_service is null, audio will be broken", __FUNCTION__);
        return;
    }

    am_service = interface_cast<com::delphi::IAudioManager>(binderInterface);
    ALOGI("%s: am_service %p", __FUNCTION__, am_service.get());

    if (am_service.get() == nullptr) {
        ALOGW("am_service is null");
        return;
    }

    // subscribe
    binder::Status status = am_service->subscribe(this);
    if (!status.isOk()) {
        ALOGE("Error subscribing to service events: %s", status.exceptionMessage().string());
    }
    usleep(50000);  // give time for Binder to finish subsription
}

android::binder::Status SoundWrapper::soundStopped(int32_t connectionId) {
    ALOGI("Sound stopped. connection Id: %d", connectionId);
    return android::binder::Status::ok();
}

SoundWrapper::~SoundWrapper() {
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        sounds.clear();
    }
}

SoundWrapper::Result SoundWrapper::play(SoundID soundid, bool continuous) {
    static SoundWrapper soundWrapperInstance;

    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        i = sounds.find(soundid);
        if (i == sounds.end()) {
            // no one has ever used this sound previously, lets create it
            sound = std::make_shared<Sound>(soundid, am_service);
            sounds[soundid] = sound;
        } else {
            // pick out the existing sound
            sound = i->second;
        }
    }

    return sound->play(continuous);
}

SoundWrapper::Result SoundWrapper::stop(SoundID soundid) {
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        i = sounds.find(soundid);
        if (i == sounds.end()) {
            // no one has ever used this sound so it has never been started -> report invalid state
            return Result::INVALID_STATE;
        } else {
            sound = i->second;
        }
    }

    return sound->stop();
}

bool SoundWrapper::isPlaying(SoundID soundid) {
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        i = sounds.find(soundid);
        if (i == sounds.end()) {
            // no one has ever used this sound
            return false;
        } else {
            const auto s = i->second->getState();
            return (s == Sound::State::Playing) || (s == Sound::State::ShallPlay) || (s == Sound::State::Starting);
        }
    }
}

void SoundWrapper::onPlayStarted(const AudioTable::SoundType type,
                                 const AudioTable::SoundComponent component,
                                 const int32_t connectionID) {
    (void)connectionID;
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    const SoundID soundid(type, component);
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        // See if we are handling this sound
        i = sounds.find(soundid);
        if (i != sounds.end()) {
            // yes we are
            sound = i->second;
        }
    }

    if (sound) {
        sound->onPlayStarted();
    }
}

void SoundWrapper::onPlayStopped(const AudioTable::SoundType type,
                                 const AudioTable::SoundComponent component,
                                 const int32_t connectionID,
                                 const int32_t reason) {
    (void)connectionID;
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    const SoundID soundid(type, component);
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        // See if we are handling this sound
        i = sounds.find(soundid);
        if (i != sounds.end()) {
            // yes we are
            sound = i->second;
        }
    }

    if (sound) {
        sound->onPlayStopped(reason);
    }
}

void SoundWrapper::onPlayFailed(const AudioTable::SoundType type,
                                const AudioTable::SoundComponent component,
                                const int32_t connectionID,
                                const int32_t error) {
    (void)connectionID;
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    const SoundID soundid(type, component);
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        // See if we are handling this sound
        i = sounds.find(soundid);
        if (i != sounds.end()) {
            // yes we are
            sound = i->second;
        }
    }

    if (sound) {
        sound->onPlayFailed(error);
    }
}

// SoundID
SoundWrapper::SoundID::SoundID(AudioTable::SoundType t, AudioTable::SoundComponent c) : type(t), component(c) {}

bool SoundWrapper::SoundID::operator<(const SoundID& rhs) const {
    const SoundID& lhs = *this;
    // We treat "type" as most significant and "component" as least significant part when sorting
    // This is needed since we use SoundID as a key in an std::map
    if (lhs.type != rhs.type) return lhs.type < rhs.type;
    return lhs.component < rhs.component;
}

#ifdef UNIT_TEST
void SoundWrapper::clearAll() { sounds.clear(); }

int SoundWrapper::getSoundState(SoundID soundid) {
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i = sounds.find(soundid);
    std::shared_ptr<Sound> sound;
    if (i != sounds.end()) {
        return (int)i->second->getState();
    } else {
        return -1;  // sound not found
    }
}
#endif

}  // namespace
