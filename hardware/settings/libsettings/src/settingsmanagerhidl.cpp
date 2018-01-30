/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "libsettings/settingsmanagerhidl.h"
#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "Settings"

namespace SettingsFramework {

namespace profilesHidl = vendor::volvocars::hardware::profiles::V1_0;

static constexpr uint64_t ISETTINGS_HAL_DEATH_COOKIE = 0xDEAD;

namespace {
uint32_t toHidl(const SettingId& key) { return static_cast<uint32_t>(key); }
settingsHidl::UserScope toHidl(const UserScope& u) { return static_cast<settingsHidl::UserScope>(u); }
SettingId fromHidl(const uint32_t& key) { return static_cast<SettingId>(key); }

std::string toString(SettingId id) { return std::to_string(static_cast<uint32_t>(id)); }
}  // namespace

SettingsManagerHidl::SettingsManagerHidl(tarmac::eventloop::IDispatcher& dispatcher) : dispatcher{dispatcher} {
    settingsHidl::ISettingsStorage::registerForNotifications("default", this);
}

SettingsHandle SettingsManagerHidl::attachSetting(
        const SettingId& name, UserScope u, std::function<void(const std::string&, ProfileIdentifier)> onSettingChanged,
        std::function<void(ProfileIdentifier p)> onSettingReset) {
    ALOGV("attachSetting key=%d, userscope=%d", name, u);

    {
        std::lock_guard<std::recursive_mutex> lock(cacheMutex_);
        auto emplaceInfo =
                settingsCache_.emplace(name, SettingsInfo{u, std::move(onSettingChanged), std::move(onSettingReset)});
        bool newInsertion = emplaceInfo.second;
        if (!newInsertion) {
            throw std::runtime_error("Mutliple instances of same setting: " + toString(name));
        }
        ALOGV("attachSetting done %d", name);
    }

    if (settingsProxy == nullptr) {
        // Will automatically be subscribed once connected
        // TODO(ee): Fix proper attach/detach behavior? (not important when we only allow once instance per
        // name)
        return 1;
    }
    ALOGV("Settingshidl::attachSetting->subscribe");
    settingsProxy->subscribe(toHidl(name), toHidl(u), this);
    ALOGV("Settingshidl::done");
    return 1;  // TODO(ee): Fix proper attach/detach behavior? (not important when we only allow once instance per name)
}

void SettingsManagerHidl::detachSetting(const SettingId& name, SettingsHandle handle) {
    (void)handle;  // TODO(ee): Fix proper attach/detach behavior? (not important when we only allow once instance per
                   // name)
    ALOGV("SettingsManagerHidl::detachSetting name=%d", name);

    {
        std::lock_guard<std::recursive_mutex> lock(cacheMutex_);
        auto i = settingsCache_.find(name);
        if (i != settingsCache_.end()) {
            settingsCache_.erase(i);
        }
        pendingUpdates.remove_if([name](auto& x) { return x->key_ == name; });
    }
    ALOGV("detachSetting remove done %d", name);

    auto settingsProxyCopy = settingsProxy;
    if (settingsProxyCopy != nullptr) {
        auto result = settingsProxyCopy->unsubscribe(toHidl(name), this);
        if (!result.isOk()) {
            ALOGW("Failed to unsubscribe setting, key=%d", name);
        }
    }
    ALOGV("detachSetting done %d", name);
}

std::string SettingsManagerHidl::getRawData(const SettingId& name, const ProfileIdentifier profid) {
    auto settingsProxyCopy = settingsProxy;
    if (settingsProxyCopy == nullptr) {
        throw std::runtime_error("ISettingsStorage not connected before get setting" + toString(name));
    }

    const auto p = static_cast<profilesHidl::ProfileIdentifier>(profid);

    std::string resultOuter;
    auto result = settingsProxyCopy->get(toHidl(name), p,
                                         [&resultOuter](andrHw::hidl_string result) { resultOuter = result; });
    if (!result.isOk()) {
        throw std::runtime_error("Failed to get setting " + toString(name) + ", error: %s" + result.description());
    }

    return resultOuter;
}

void SettingsManagerHidl::setRawData(const SettingId& name, ProfileIdentifier profid, const std::string& data) {
    auto settingsProxyCopy = settingsProxy;
    if (settingsProxyCopy == nullptr) {
        throw std::runtime_error("ISettingsStorage not connected before set setting " + toString(name));
        return;
    }

    const auto p = static_cast<profilesHidl::ProfileIdentifier>(profid);

    andrHw::hidl_string dataToSend = data;
    auto result = settingsProxyCopy->set(toHidl(name), p, dataToSend);
    if (!result.isOk()) {
        ALOGE("Failed to set setting %d, error: %s", name, result.description().c_str());
    }
}

andrHw::Return<void> SettingsManagerHidl::onRegistration(const andrHw::hidl_string& fqName,
                                                         const andrHw::hidl_string& name, bool preexisting) {
    ALOGV("onRegistration, fqName=%s, name=%s, preexisting=%d", fqName.c_str(), name.c_str(), preexisting);

    settingsProxy = settingsHidl::ISettingsStorage::getService();
    if (settingsProxy == nullptr) {
        ALOGE("Could not contact ISettingsStorage HAL");
        return andrHw::Return<void>();
    }
    ALOGV("Successfully connected to settings hidl, now link to death");
    auto result = settingsProxy->linkToDeath(this, ISETTINGS_HAL_DEATH_COOKIE);
    if (!result.isOk()) {
        ALOGE("ISettingsStorage link to death error: %s", result.description().c_str());
        return andrHw::Return<void>();
    }
    ALOGV("Link to death ok");

    reloadAllSettings(*settingsProxy);

    ALOGV("Link to death reloadsettings done");

    return andrHw::Return<void>();
}

void SettingsManagerHidl::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) {
    (void)who;
    if (cookie != ISETTINGS_HAL_DEATH_COOKIE) {
        return;
    }
    ALOGE("SettingsManager disconnected");
    settingsProxy = nullptr;
}

andrHw::Return<void> SettingsManagerHidl::settingsForCurrentUserChanged(
        const uint32_t key, settingsHidl::SettingsChangeReason reason, profilesHidl::ProfileIdentifier currentProfile) {
    const auto name = fromHidl(key);
    ALOGV("settingsForCurrentUserChanged, name=%d", name);
    std::string data;
    if (reason == settingsHidl::SettingsChangeReason::ProfileChange) {
        ALOGV("Settingschange, get data");
        data = getRawData(name, static_cast<ProfileIdentifier>(currentProfile));
        ALOGV("Settingschange, onSettingChanged done");
    }
    {
        std::unique_lock<std::recursive_mutex> cacheMutex_;
        pendingUpdates.emplace_back(
                std::make_unique<SettingsUpdate>(name, static_cast<ProfileIdentifier>(currentProfile), data, reason));
    }

    android::wp<SettingsManagerHidl> weak_this{this};
    dispatcher.Enqueue([weak_this]() mutable {
        android::sp<SettingsManagerHidl> lockWeak = weak_this.promote();
        if (lockWeak != nullptr) {
            lockWeak->dispatchPendingSettingsUpdatesOnAppThread();
        }
    });

    return andrHw::Return<void>();
}

void SettingsManagerHidl::reloadAllSettings(settingsHidl::ISettingsStorage& settings_proxy) {
    std::lock_guard<std::recursive_mutex> lock(cacheMutex_);

    ALOGV("reloadAllSettings, size=%lu", settingsCache_.size());
    for (auto& it : settingsCache_) {
        const SettingId name = it.first;
        const SettingsInfo& info = it.second;
        ALOGV("reloadAllSettings, name=%d", name);
        // Subscribe shall ALWAYS call settingsForCurrentUserChanged first time
        auto result = settings_proxy.subscribe(toHidl(name), toHidl(info.userScope), this);
        ALOGW_IF(!result.isOk(), "Failed to subscribe to setting %d, error=%s", name, result.description().c_str());
    }
}

// Always run from Dispatcher thread, hence no risk of dispatching into Detached settings
void SettingsManagerHidl::dispatchPendingSettingsUpdatesOnAppThread() {
    std::list<std::unique_ptr<SettingsUpdate>> updatesToDispatch;
    {
        std::unique_lock<std::recursive_mutex> cacheMutex_;
        updatesToDispatch = std::move(pendingUpdates);
        pendingUpdates.clear();
    }

    ALOGV("Dispatching %lu settings", updatesToDispatch.size());
    for (const auto& u : updatesToDispatch) {
        auto it = settingsCache_.find(u->key_);
        if (it == settingsCache_.end()) {
            continue;
        }
        if (u->reason_ == settingsHidl::SettingsChangeReason::Reset) {
            ALOGV("Settingschange, settingsreset");
            it->second.onSettingReset(u->profileId_);
        } else {
            ALOGV("Settingschange, get data");
            it->second.onSettingChanged(u->data_, u->profileId_);
            ALOGV("Settingschange, onSettingChanged done");
        }
    }
}

}  // namespace SettingsFramework