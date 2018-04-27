/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "soundwrapper.h"
#include <signal.h>
#include <memory>
#include <mutex>
#include <string>

#undef LOG_TAG
#define LOG_TAG "SoundNotificationWrp"

namespace SoundNotifications {

android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> SoundWrapper::am_service;
static const std::chrono::milliseconds kRestartTimeout = std::chrono::milliseconds(5);
static const std::chrono::milliseconds kStartWaitTimeout = std::chrono::milliseconds(10);

// ==========================================================
// implementation Sound
Sound::Sound(const SoundWrapper::SoundID& soundID,
             const android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>& service)
    : _state(State::Idle),
      connectionID(0),
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
    job_id_ = 0;
    play();
}

void Sound::cancelTimer() {
    if (job_id_ > 0) {
        restartTimer.Cancel(job_id_);
        job_id_ = 0;
    }
}

/*
 * Called on disconnected callback from AM
*/
void Sound::onDisconnected() {
    connectionID = 0;
    // make sure state is idle
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    _state = State::Idle;
}

SoundWrapper::SoundID Sound::getSoundID() const {
    return _soundID;
}

void Sound::play() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);

    using namespace vendor::delphi::audiomanager::V1_0;

    // cancel any running timer to be able to accept new play request while we are working on the previous play request.
    cancelTimer();

    switch (_state) {
        case State::Idle: {
            _state = State::Starting;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            ALOGI("Sound::play() play sound notif sound type=%d component=%d",
                  static_cast<int32_t>(_soundID.type),
                  static_cast<int32_t>(_soundID.component));
            android::hardware::hidl_string soundTypeHidl(AudioTable::getSoundTypeName(_soundID.type));
            android::hardware::hidl_string soundCompHidl(AudioTable::getSoundComponentName(_soundID.component));

            android::hardware::Return<void> ret =
                    am_service->playSound(soundTypeHidl, soundCompHidl, [&](AMStatus s, uint32_t cId) {
                        if (s == AMStatus::OK) {
                            connectionID = cId;
                            SoundWrapper::registerConnection(this, connectionID);
                            ALOGD("Sound::play Idle->Starting %s. Connection ID %d", _name.c_str(), connectionID);
                        } else {
                            ALOGW("Sound::play failed to playSound %s. AMStatus: %i", _name.c_str(), s);
                            _state = State::Idle;  // stay in Idle
                            if (s == AMStatus::CONNECTION_NOT_ALLOWED) {
                                // more prio sound ongoing restart playback after a while
                                job_id_ = restartTimer.EnqueueWithDelay(kRestartTimeout, [this]() { onTimeout(); });
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
            ALOGD("Sound::play invalid state Playing %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Starting:
            ALOGD("Sound::play invalid state Starting %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Stopping:
            job_id_ = restartTimer.EnqueueWithDelay(kStartWaitTimeout, [this]() { onTimeout(); });
            ALOGD("Sound::play Waiting to enter in the Idle %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        default:
            ALOGW("Sound::play Invalid state %d %s. Connection ID %d", (int)_state, _name.c_str(), connectionID);
            break;
    }
}

void Sound::stop() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);

    // cancel any running timer to stop the sound.
    cancelTimer();

    switch (_state) {
        case State::Idle:
            ALOGD("Sound::stop invalid state Idle %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Starting:
        case State::Playing: {
            _state = State::Stopping;  // we set this here since we dont know if playSound maybe does the callback
                                       // directly
            using namespace vendor::delphi::audiomanager::V1_0;

            ::android::hardware::Return<AMStatus> status = am_service->stopSound(connectionID);

            if (status.isOk()) {
                ALOGD("Sound::stop Playing->Stopping %s. Connection ID %d", _name.c_str(), connectionID);
            } else {
                ALOGE("Sound::stop failed to stopSound %s, jumping to Idle %s. Connection ID %d",
                      status.description().c_str(),
                      _name.c_str(),
                      connectionID);
            }
            break;
        }

        case State::Stopping:
            ALOGD("Sound::stop invalid state Stopping %s. Connection ID %d", _name.c_str(), connectionID);
            break;
    }
}

std::string Sound::name() const {
    return _name;
}

uint32_t Sound::getConnectionID() const {
    return connectionID;
}

void Sound::onPlayStarted() {
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    switch (_state) {
        case State::Idle:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state Idle %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Playing:
            // Do nothing
            ALOGD("Sound::onPlayStarted invalid state Playing %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Starting:
            _state = State::Playing;
            ALOGD("Sound::onPlayStarted Starting->Playing %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Stopping:
            stop();
            break;

        default:
            ALOGE("Sound::onPlayStarted Invalid state %d %s. Connection ID %d",
                  (int)_state,
                  _name.c_str(),
                  connectionID);
            break;
    }
}

void Sound::onPlayStopped(int32_t reason) {
    ALOGD("Sound::onPlayStopped, reason %d %s", (int)reason, _name.c_str());
    std::lock_guard<std::recursive_mutex> safeLock(_stateMutex);
    if (job_id_ > 0) {
        restartTimer.Cancel(job_id_);
        job_id_ = 0;
    }
    switch (_state) {
        case State::Idle:
            // Do nothing
            ALOGD("Sound::onPlayStopped invalid state Idle %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Playing:
            _state = State::Stopping;
            ALOGD("Sound::onPlayStopped Playing->Idle %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        case State::Starting:
            _state = State::Idle;
            break;

        case State::Stopping:
            ALOGW("Sound::onPlayStopped Not Expected ever %s. Connection ID %d", _name.c_str(), connectionID);
            break;

        default:
            ALOGE("Sound::onPlayStopped Invalid state %d %s. Connection ID %d",
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
static std::map<uint32_t, std::shared_ptr<Sound>> soundIds;
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

    android::hardware::hidl_death_recipient* this_as_recipient = this;
    am_service->linkToDeath(this_as_recipient, 0);

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
    if (instance()->getInitialized()) {
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
}

void SoundWrapper::registerConnection(Sound* sound, uint32_t connectionID) {
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

bool SoundWrapper::getInitialized() const {
    return initialized.load();
}

::android::hardware::Return<void> SoundWrapper::onDisconnected(uint32_t connectionID) {
    ALOGD("%s %d", __FUNCTION__, connectionID);
    std::lock_guard<std::recursive_mutex> safeLock(soundIdsMutex);
    (void)safeLock;
    std::map<uint32_t, std::shared_ptr<Sound>>::iterator i = soundIds.find(static_cast<uint32_t>(connectionID));
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
    std::map<uint32_t, std::shared_ptr<Sound>>::iterator i = soundIds.find(static_cast<uint32_t>(connectionID));
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
    std::map<uint32_t, std::shared_ptr<Sound>>::iterator i = soundIds.find(static_cast<uint32_t>(connectionID));
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

void SoundWrapper::serviceDied(uint64_t cookie, const ::android::wp<::android::hidl::base::V1_0::IBase>& who) {
    (void)who;
    ALOGE("AM Service just died! %ld. Exiting...", cookie);
    initialized = false;
    // TEMPORARY solution to prevent crash in soundnotifications when parent audio service crashed
    kill(getpid(), SIGTERM);
    // Halt execution and wait for TERM-handler to do its work.
    pause();
    // init(::android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>());
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

uint32_t SoundWrapper::getConnectionID(SoundID soundid) {
    auto i = sounds.find(soundid);
    std::shared_ptr<Sound> sound;
    if (i != sounds.end()) {
        return i->second->getConnectionID();
    } else {
        return 0;  // sound not found
    }
}

#endif

}  // namespace SoundNotifications
