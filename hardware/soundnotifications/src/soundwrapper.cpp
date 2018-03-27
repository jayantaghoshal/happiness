/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "soundwrapper.h"
#include <memory>
#include <mutex>
#include <string>

#undef LOG_TAG
#define LOG_TAG "SoundNotificationWrp"

namespace SoundNotifications {

android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> SoundWrapper::am_service;
static const std::chrono::milliseconds kRestartTimeout = std::chrono::milliseconds(5);

// ==========================================================
// implementation Sound
Sound::Sound(const SoundWrapper::SoundID& soundID,
             const android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>& service)
    : _state(State::Idle),
      connectionID(-1),
      _soundID(soundID),
      _name("invalid"),
      am_service(service),
      restartTimer{tarmac::eventloop::IDispatcher::GetDefaultDispatcher()} {
    try {
        _name = (AudioTable::getSourceName(soundID.type, soundID.component));
        //(AudioTable::getSourceName(soundID.type, soundID.component));
    } catch (std::invalid_argument iaex) {
        ALOGW("Invalid combination of Type and Component");
    }
}

void Sound::onTimeout() {
    ALOGD("Sound::onTimeout %s", _name.c_str());
    play();
}

/*
 * Called on disconnected callback from AM
*/
void Sound::onDisconnected() {
    connectionID = -1;
    // make sure state is idle
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    _state = State::Idle;
}

void Sound::setState(int state) {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    _state = static_cast<State>(state);
}

SoundWrapper::SoundID Sound::getSoundID() const { return _soundID; }

void Sound::play() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);

    using namespace vendor::delphi::audiomanager::V1_0;

    switch (_state) {
        case State::Idle: {
            _state = State::Starting;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            ALOGI("Sound::play() play sound notif sound type=%d component=%d",
                  static_cast<int32_t>(_soundID.type),
                  static_cast<int32_t>(_soundID.component));
            android::hardware::Return<void> ret = am_service->playSound(
                    static_cast<int32_t>(_soundID.type),
                    static_cast<int32_t>(_soundID.component),
                    [&](AMStatus s, int64_t cId) {
                        if (s == AMStatus::OK) {
                            connectionID = cId;
                            SoundWrapper::registerConnection(this, connectionID);
                            ALOGD("Sound::play Idle->Starting %s. Connection ID %li", _name.c_str(), connectionID);
                        } else {
                            ALOGW("Sound::play failed to playSound %s. AMStatus: %i", _name.c_str(), s);
                            _state = State::Idle;  // stay in Idle
                            if (s == AMStatus::CONNECTION_NOT_ALLOWED) {
                                // more prio sound ongoing restart playback after a while
                                restartTimer.EnqueueWithDelay(kRestartTimeout, [this]() { onTimeout(); });
                            }
                        }
                    });

            if (!ret.isOk()) {
                ALOGW("Sound::play failed to playSound %s. Error: %s", _name.c_str(), ret.description().c_str());
                _state = State::Idle;  // stay in Idle
            }
            break;
        }
        case State::Playing:
            ALOGD("Sound::play invalid state Playing %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Starting:
            ALOGD("Sound::play invalid state Starting %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Stopping:
            _state = State::Idle;
            restartTimer.Enqueue([this]() { onTimeout(); });
            ALOGD("Sound::play Stopping->Starting %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        default:
            ALOGW("Sound::play Invalid state %d %s. Connection ID %li", (int)_state, _name.c_str(), connectionID);
            break;
    }
}

void Sound::stop() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            ALOGD("Sound::stop invalid state Idle %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Starting:
        case State::Playing: {
            _state = State::Stopping;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            using namespace vendor::delphi::audiomanager::V1_0;

            ::android::hardware::Return<AMStatus> status = am_service->stopSound(connectionID);

            _state = State::Idle;  // got to Idle
            if (status.isOk()) {
                ALOGD("Sound::stop Playing->Stopping %s. Connection ID %li", _name.c_str(), connectionID);
            } else {
                ALOGE("Sound::stop failed to stopSound %s, jumping to Idle %s. Connection ID %li",
                      status.description().c_str(),
                      _name.c_str(),
                      connectionID);
            }
            break;
        }

        case State::Stopping:
            ALOGD("Sound::stop invalid state Stopping %s. Connection ID %li", _name.c_str(), connectionID);
            break;
    }
}

std::string Sound::name() const { return _name; }

int64_t Sound::getConnectionID() const { return connectionID; }

void Sound::onPlayStarted() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state Idle %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Playing:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state Playing %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Starting:
            _state = State::Playing;
            ALOGD("Sound::onPlayStarted Starting->Playing %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Stopping:
            stop();
            break;

        default:
            ALOGE("Sound::onPlayStarted Invalid state %d %s. Connection ID %li",
                  (int)_state,
                  _name.c_str(),
                  connectionID);
            break;
    }
}

void Sound::onPlayStopped(int32_t reason) {
    ALOGD("Sound::onPlayStopped, reason %d %s", (int)reason, _name.c_str());
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            // Do nothing
            ALOGD("Sound::onPlayStopped invalid state Idle %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Playing:
            _state = State::Idle;
            ALOGD("Sound::onPlayStopped Playing->Idle %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        case State::Starting:
            _state = State::Idle;
            restartTimer.Enqueue([this]() { onTimeout(); });
            break;

        case State::Stopping:
            _state = State::Idle;
            ALOGD("Sound::onPlayStopped Stopping->Idle %s. Connection ID %li", _name.c_str(), connectionID);
            break;

        default:
            ALOGE("Sound::onPlayStopped Invalid state %d %s. Connection ID %li",
                  (int)_state,
                  _name.c_str(),
                  connectionID);
            break;
    }
}

// ==========================================================
// implementation SoundWrapper

// Only used by the SoundWrapper class
static std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>> sounds;
static std::map<int64_t, std::shared_ptr<Sound>> soundIds;
static std::recursive_mutex soundsMutex;
static std::recursive_mutex soundIdsMutex;

bool SoundWrapper::init(::android::sp<IAudioManager> service) {
    // for unit testing

    if (service != nullptr) {
        if (service != am_service) {
            am_service = service;
        }
    } else {
        while (am_service.get() == nullptr) {
            am_service = IAudioManager::getService();

            if (am_service == nullptr) {
                ALOGW("%s: getService(am_service) failed", __func__);
                sleep(1);
            }
        }
    }

    // subscribe
    android::hardware::Return<void> ret = am_service->subscribe(::android::sp<IAudioManagerCallback>(this));
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

void SoundWrapper::play(SoundID soundid) {
    // call instance here. This will instantiate soundwrapper if its not done yet
    if (instance()->getInitialized()) {
        std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
        std::shared_ptr<Sound> sound;
        {  // lock scope
            std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
            (void)safeLock;

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

        sound->play();
    }
}

void SoundWrapper::stop(SoundID soundid) {
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        (void)safeLock;
        i = sounds.find(soundid);
        if (i == sounds.end()) {
            // no one has ever used this sound so it has never been started -> report invalid state
            return;
        } else {
            sound = i->second;
        }
    }

    sound->stop();
}

void SoundWrapper::registerConnection(Sound* sound, int64_t connectionID) {
    std::lock_guard<std::recursive_mutex> safeLock(soundIdsMutex);
    (void)safeLock;
    if (sound == nullptr) {
        return;
    }
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound_;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        (void)safeLock;
        i = sounds.find(sound->getSoundID());
        if (i != sounds.end()) {
            soundIds[connectionID] = std::shared_ptr<Sound>(i->second);
        }
    }
}

bool SoundWrapper::isPlaying(SoundID soundid) {
    std::map<SoundWrapper::SoundID, std::shared_ptr<Sound>>::iterator i;
    std::shared_ptr<Sound> sound;
    {  // lock scope
        std::lock_guard<std::recursive_mutex> safeLock(soundsMutex);
        (void)safeLock;
        i = sounds.find(soundid);
        if (i == sounds.end()) {
            // no one has ever used this sound
            return false;
        } else {
            const auto s = i->second->getState();
            return (s == Sound::State::Playing) || (s == Sound::State::Starting);
        }
    }
}

SoundWrapper* SoundWrapper::instance() {
    static SoundWrapper soundWrapperInstance;
    return &soundWrapperInstance;
}

SoundWrapper::~SoundWrapper() {
    ALOGD("%s %d", __FUNCTION__, initialized.load());
    if (initialized.load()) {
        am_service->unsubscribe(::android::sp<IAudioManagerCallback>(this));
    }

    initialized = false;
}

bool SoundWrapper::getInitialized() const { return initialized.load(); }

::android::hardware::Return<void> SoundWrapper::onDisconnected(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    std::lock_guard<std::recursive_mutex> safeLock(soundIdsMutex);
    (void)safeLock;
    std::map<int64_t, std::shared_ptr<Sound>>::iterator i = soundIds.find(static_cast<int64_t>(connectionID));
    if (i != soundIds.end()) {
        if (i->second->getConnectionID() == connectionID) {
            i->second->onDisconnected();
        }
    }
    return android::hardware::Status::fromStatusT(android::OK);
}

::android::hardware::Return<void> SoundWrapper::onConnected(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    std::lock_guard<std::recursive_mutex> safeLock(soundIdsMutex);
    (void)safeLock;

    return android::hardware::Status::fromStatusT(android::OK);
}

::android::hardware::Return<void> SoundWrapper::onWavFileFinished(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    std::lock_guard<std::recursive_mutex> safeLock(soundIdsMutex);
    (void)safeLock;
    std::map<int64_t, std::shared_ptr<Sound>>::iterator i = soundIds.find(static_cast<int64_t>(connectionID));
    if (i != soundIds.end()) {
        if (i->second->getConnectionID() == connectionID) {
            i->second->onPlayStopped(0);
        }
    }
    return android::hardware::Status::fromStatusT(android::OK);
}

::android::hardware::Return<void> SoundWrapper::onRampedIn(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    std::lock_guard<std::recursive_mutex> safeLock(soundIdsMutex);
    (void)safeLock;
    std::map<int64_t, std::shared_ptr<Sound>>::iterator i = soundIds.find(static_cast<int64_t>(connectionID));
    if (i != soundIds.end()) {
        if (i->second->getConnectionID() == connectionID) {
            i->second->onPlayStarted();
        }
    }
    return android::hardware::Status::fromStatusT(android::OK);
}

::android::hardware::Return<void> SoundWrapper::ackSetSinkVolumeChange(uint32_t sinkId, int16_t volume) {
    ALOGD("%s sink: %d, volume: %d", __FUNCTION__, sinkId, volume);
    return android::hardware::Status::fromStatusT(android::OK);
}

// SoundID
SoundWrapper::SoundID::SoundID(AudioTable::SoundType t, AudioTable::SoundComponent c, bool valid)
    : type(t), component(c), isValid(valid) {}

bool SoundWrapper::SoundID::operator<(const SoundID& s) const {
    const SoundID& lhs = *this;
    // We treat "type" as most significant and "component" as least significant part when sorting
    // This is needed since we use SoundID as a key in an std::map
    if (lhs.type != s.type) return lhs.type < s.type;
    return lhs.component < s.component;
}

SoundWrapper::SoundID& SoundWrapper::SoundID::operator=(SoundWrapper::SoundID other) {
    type = other.type;
    component = other.component;
    isValid = other.isValid;
    return *this;
}

#ifdef UNIT_TEST
void SoundWrapper::clearAll() {
    ALOGD("%s", __FUNCTION__);
    sounds.clear();
    soundIds.clear();
}

int SoundWrapper::getSoundState(SoundID soundid) {
    auto i = sounds.find(soundid);
    std::shared_ptr<Sound> sound;
    if (i != sounds.end()) {
        return static_cast<int>(i->second->getState());
    } else {
        return -1;  // sound not found
    }
}

int64_t SoundWrapper::getConnectionID(SoundID soundid) {
    auto i = sounds.find(soundid);
    std::shared_ptr<Sound> sound;
    if (i != sounds.end()) {
        return i->second->getConnectionID();
    } else {
        return -1;  // sound not found
    }
}

void SoundWrapper::setSoundState(SoundID soundid, int state) {
    auto i = sounds.find(soundid);
    std::shared_ptr<Sound> sound;
    if (i != sounds.end()) {
        i->second->setState(state);
        return;
    } else {
        return;  // sound not found
    }
}

#endif

}  // namespace SoundNotifications
