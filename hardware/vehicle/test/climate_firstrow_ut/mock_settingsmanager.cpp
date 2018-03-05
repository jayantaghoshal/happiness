/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "mock_settingsmanager.h"

SettingsFramework::SettingsHandle MockSettingsManager::attachSetting(
        const SettingId& name, SettingsFramework::UserScope u,
        std::function<void(const std::string&, SettingsFramework::ProfileIdentifier)> onSettingChanged,
        std::function<void(SettingsFramework::ProfileIdentifier p)> onSettingReset) {
    callbacks[name] = onSettingChanged;
    if (u == SettingsFramework::UserScope::NOT_USER_RELATED) {
        onSettingReset(SettingsFramework::ProfileIdentifier::None);
    } else {
        onSettingReset(SettingsFramework::ProfileIdentifier::Profile1);
    }
    return {};
}
void MockSettingsManager::detachSetting(const SettingId& name, SettingsFramework::SettingsHandle handle) {}

std::string MockSettingsManager::getRawData(const SettingId& name, SettingsFramework::ProfileIdentifier profid) {
    return storage[name];
}
void MockSettingsManager::setRawData(const SettingId& name, SettingsFramework::ProfileIdentifier profid,
                                     const std::string& data) {
    storage[name] = data;
    callbacks[name](data, profid);
}