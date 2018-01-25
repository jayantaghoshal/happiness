/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "soundwrapper.h"
#include <memory>
#include <mutex>

#undef LOG_TAG
#define LOG_TAG "SoundNotificationWrp"

namespace SoundNotifications {

android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> SoundWrapper::am_service;

/*
 * class to handle each unique sound that shall be played
 * Instances are only created if needed.
 */
class Sound {
  public:
    // Start playing the given sound id
    SoundWrapper::Result play();

    // Stop playing the given sound id. This should probably ONLY be called for Non-flank triggerd sounds
    SoundWrapper::Result stop();

    Sound(const SoundWrapper::SoundID& soundID,
          const android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>& service);

    // play callbacks but only for this sound
    void onPlayStarted();
    void onPlayStopped(int32_t reason);
    void onPlayFailed(int32_t error);

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

    int64_t getConnectionID() const;

  private:
    State _state;
    std::recursive_mutex _stateMutex;

    // Which soundid we "are"
    int64_t connectionID;

    SoundWrapper::SoundID _soundID;
    const std::string _name;
    android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> am_service;
};

// ==========================================================
// implementation Sound
Sound::Sound(const SoundWrapper::SoundID& soundID,
             const android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>& service)
    : _state(State::Idle),
      _soundID(soundID),
      _name(AudioTable::getSourceName(soundID.type, soundID.component)),
      am_service(service) {}

SoundWrapper::Result Sound::play() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);

    using namespace vendor::delphi::audiomanager::V1_0;

    switch (_state) {
        case State::Idle: {
            _state = State::Starting;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            android::hardware::Return<void> ret =
                    am_service->playSound(static_cast<int32_t>(_soundID.type),
                                          static_cast<int32_t>(_soundID.component),
                                          [&](AMStatus s, int64_t cId) {
                                              connectionID = cId;
                                              if (s == AMStatus::OK) {
                                                  ALOGV("Sound::play Idle->Starting %s", _name.c_str());
                                                  _state = State::Idle;
                                              } else {
                                                  ALOGW("Sound::play failed to playSound %s. Error: %s",
                                                        _name.c_str(),
                                                        ret.description().c_str());
                                                  _state = State::Idle;  // stay in Idle
                                              }
                                          });

            if (!ret.isOk()) {
                ALOGW("Sound::play failed to playSound %s. Error: %s", _name.c_str(), ret.description().c_str());
                _state = State::Idle;  // stay in Idle
                return SoundWrapper::Result::PLAY_FAILED;
            } else {
                return SoundWrapper::Result::OK;
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
            using namespace vendor::delphi::audiomanager::V1_0;

            ::android::hardware::Return<AMStatus> status = am_service->stopSound(connectionID);

            if (status.isOk()) {
                ALOGV("Sound::stop Playing->Stopping %s", _name.c_str());
                return SoundWrapper::Result::OK;
            } else {
                ALOGE("Sound::stop failed to stopSound %s, jumping to Idle %s",
                      status.description().c_str(),
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
            ALOGV("Sound::stop ShallPlay->Idle %s", _name.c_str());
            return SoundWrapper::Result::OK;
    }
    return SoundWrapper::Result::INVALID_STATE;
}

std::string Sound::name() const { return _name; }

int64_t Sound::getConnectionID() const { return connectionID; }

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
            _state = State::Idle;
            ALOGV("Sound::onPlayStopped Playing->Idle %s", _name.c_str());
            break;

        case State::Starting:
            if (play() != SoundWrapper::Result::OK) {
                _state = State::Idle;
                ALOGE("Sound::onPlayStopped failed to playSound %s", _name.c_str());
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

// ==========================================================
// implementation SoundWrapper

// Only used by the SoundWrapper class
static std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>> sounds;
static std::recursive_mutex soundsMutex;

SoundWrapper::SoundWrapper() {}

bool SoundWrapper::init(vendor::delphi::audiomanager::V1_0::IAudioManager* service) {
    // for unit testing
    if (service != nullptr) {
        am_service = service;
        initialized = true;
        return true;
    }

    using namespace android;

    while (am_service == nullptr) {
        am_service = vendor::delphi::audiomanager::V1_0::IAudioManager::getService();

        if (am_service == nullptr) {
            ALOGW("%s: getService(am_service) failed", __func__);
            sleep(1);
        }
    }
    using namespace vendor::delphi::audiomanager::V1_0;

    // subscribe
    hardware::Return<void> ret = am_service->subscribe(this);
    if (!ret.isOk()) {
        ALOGE("Error subscribing to service events: %s", ret.description().c_str());
    }
    initialized = true;
    return true;
}

void SoundWrapper::cleanup() {
    std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
    (void)safeLock;
    sounds.clear();
}

::android::hardware::Return<void> SoundWrapper::onDisconnected(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    return android::hardware::Return<void>();
}

::android::hardware::Return<void> SoundWrapper::onConnected(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    return android::hardware::Return<void>();
}

::android::hardware::Return<void> SoundWrapper::onWavFileFinished(uint32_t cId) {
    ALOGD("%s %d", __FUNCTION__, cId);
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        for (i = sounds.begin(); i != sounds.end(); ++i) {
            std::shared_ptr<Sound> sound = i->second;
            if (sound && sound->getConnectionID() == cId) {
                sound->onPlayStopped(0);
                break;
            }
        }
    }
    return android::hardware::Return<void>();
}

::android::hardware::Return<void> SoundWrapper::onRampedIn(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        for (i = sounds.begin(); i != sounds.end(); ++i) {
            std::shared_ptr<Sound> sound = i->second;
            if (sound && sound->getConnectionID() == connectionID) {
                sound->onPlayStarted();
                break;
            }
        }
    }

    return android::hardware::Return<void>();
}

::android::hardware::Return<void> SoundWrapper::ackSetSinkVolumeChange(uint32_t sinkId, int16_t volume) {
    ALOGD("%s sink: %d, volume: %d", __FUNCTION__, sinkId, volume);
    return android::hardware::Return<void>();
}

SoundWrapper::~SoundWrapper() {}

SoundWrapper::Result SoundWrapper::play(SoundID soundid) {
    // call instance here. This will instantiate soundwrapper if its not done yet
    if (instance()->getInitialized()) {
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

        return sound->play();
    } else {
        return SoundWrapper::Result::PLAY_FAILED;
    }
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

SoundWrapper* SoundWrapper::instance() {
    static SoundWrapper soundWrapperInstance;
    return &soundWrapperInstance;
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

bool SoundWrapper::getInitialized() const { return initialized.load(); }

// SoundID
SoundWrapper::SoundID::SoundID(AudioTable::SoundType t, AudioTable::SoundComponent c) : type(t), component(c) {}

bool SoundWrapper::SoundID::operator<(const SoundID& s) const {
    const SoundID& lhs = *this;
    // We treat "type" as most significant and "component" as least significant part when sorting
    // This is needed since we use SoundID as a key in an std::map
    if (lhs.type != s.type) return lhs.type < s.type;
    return lhs.component < s.component;
}

#ifdef UNIT_TEST
void SoundWrapper::clearAll() { sounds.clear(); }

int SoundWrapper::getSoundState(SoundID soundid) {
    auto i = sounds.find(soundid);
    std::shared_ptr<Sound> sound;
    if (i != sounds.end()) {
        return static_cast<int>(i->second->getState());
    } else {
        return -1;  // sound not found
    }
}
#endif

}  // namespace SoundNotifications
