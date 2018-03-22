/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "mock_settingsmanager.h"
#include "ivi-logging.h"

SettingsFramework::SettingsHandle MockSettingsManager::attachSetting(
        const SettingId& name,
        SettingsFramework::UserScope u,
        std::function<void(const std::string&, SettingsFramework::ProfileIdentifier)> onSettingChanged,
        std::function<void(SettingsFramework::ProfileIdentifier p)> onSettingReset) {
    callbacks[name] = onSettingChanged;

    log_info() << "MOCKSETTINGS attachSetting" << static_cast<int>(name);

    // TODO: Calling callback synchronously in Setting constructor is not done in real SettingsManager
    //      Hack to make old unit tests work
    //      It should be called "some time soon" after construction but not guaranteed to be synchronous
    auto it = storage.find(name);
    if (it != storage.end()) {
        if (u == SettingsFramework::UserScope::NOT_USER_RELATED) {
            onSettingChanged(it->second, SettingsFramework::ProfileIdentifier::None);
        } else {
            onSettingChanged(it->second, SettingsFramework::ProfileIdentifier::Profile1);
        }
    } else {
        if (u == SettingsFramework::UserScope::NOT_USER_RELATED) {
            onSettingReset(SettingsFramework::ProfileIdentifier::None);
        } else {
            onSettingReset(SettingsFramework::ProfileIdentifier::Profile1);
        }
    }
    return {};
}
void MockSettingsManager::detachSetting(const SettingId& name, SettingsFramework::SettingsHandle handle) {}

std::string MockSettingsManager::getRawData(const SettingId& name, SettingsFramework::ProfileIdentifier profid) {
    return storage[name];
}
void MockSettingsManager::setRawData(const SettingId& name,
                                     SettingsFramework::ProfileIdentifier profid,
                                     const std::string& data) {
    log_info() << "MOCKSETTINGS setRawData" << static_cast<int>(name) << "data:" << data;
    storage[name] = data;
    auto it = callbacks.find(name);
    if (it != callbacks.end()) {
        log_info() << "MOCKSETTINGS found someone to notify";

        // TODO: ????? NOTE: No need to notify as Setting<T> does that by itself and we only allow one Setting per
        // SettingId
        // it->second(data, profid);
    } else {
        log_info() << "MOCKSETTINGS nobody to notify";
    }
    log_info() << "MOCKSETTINGS setRawData DONE";
}