/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <IDispatcher.h>
#include <vendor/volvocars/hardware/settings/1.0/ISettingsListener.h>
#include <vendor/volvocars/hardware/settings/1.0/ISettingsStorage.h>
#include <list>
#include "libsettings/settingsmanager.h"

namespace SettingsFramework {

namespace andrHw = ::android::hardware;
namespace settingsHidl = vendor::volvocars::hardware::settings::V1_0;

class SettingsUpdate {
  public:
    SettingsUpdate(const std::string& key, const ProfileIdentifier profileId, const std::string& data,
                   const settingsHidl::SettingsChangeReason reason)
        : key{key}, profileId{profileId}, data{data}, reason{reason} {}
    const std::string key;
    const ProfileIdentifier profileId;
    const std::string data;
    const settingsHidl::SettingsChangeReason reason;
};

class SettingsManagerHidl : public SettingsManager,
                            public ::android::hidl::manager::V1_0::IServiceNotification,
                            public settingsHidl::ISettingsListener,
                            public andrHw::hidl_death_recipient {
  public:
    SettingsManagerHidl(tarmac::eventloop::IDispatcher& dispatcher);

    virtual SettingsHandle attachSetting(const std::string& name, UserScope u,
                                         std::function<void(const std::string&, ProfileIdentifier)> onSettingChanged,
                                         std::function<void(ProfileIdentifier p)> onSettingReset);
    void detachSetting(const std::string& name, SettingsHandle handle) override;
    std::string getRawData(const std::string& name, ProfileIdentifier profid) override;
    void setRawData(const std::string& name, ProfileIdentifier profid, const std::string& data) override;

    andrHw::Return<void> settingsForCurrentUserChanged(
            const andrHw::hidl_string& key, settingsHidl::SettingsChangeReason reason,
            ::vendor::volvocars::hardware::profiles::V1_0::ProfileIdentifier currentProfile) override;

    andrHw::Return<void> onRegistration(const andrHw::hidl_string& fqName, const andrHw::hidl_string& name,
                                        bool preexisting) override;
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

  private:
    void reloadAllSettings(android::sp<settingsHidl::ISettingsStorage>& settingsProxyCopy);
    void dispatchPendingSettingsUpdatesOnAppThread();

    struct SettingsInfo {
        UserScope userScope;
        std::function<void(const std::string&, ProfileIdentifier)> onSettingChanged;
        std::function<void(ProfileIdentifier p)> onSettingReset;
    };

    tarmac::eventloop::IDispatcher& dispatcher;
    std::list<std::unique_ptr<SettingsUpdate>> pendingUpdates;  // Only access with cacheMutex
    std::map<std::string, SettingsInfo> settingsCache_;         // Only access with cacheMutex
    // recursive_mutex due to reloadAllSettings-->subscribe-->settingsForCurrentUserChanged
    std::recursive_mutex cacheMutex_;
    ::android::sp<settingsHidl::ISettingsStorage>
            settingsProxy;  // Take sp-copy when check+use from app-thread, written by hidl-thread
};

}  // namespace SettingsFramework