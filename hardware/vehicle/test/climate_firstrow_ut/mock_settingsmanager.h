/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <libsettings/setting.h>
#include <libsettings/settingsmanager.h>

template <typename T, SettingsFramework::UserScope userScope>
class CompatSetting : public SettingsFramework::Setting<T, userScope> {
  public:
    CompatSetting(SettingId id, T defaultValue, android::sp<SettingsFramework::SettingsManager> sm)
        : SettingsFramework::Setting<T, userScope>{id, defaultValue, sm} {}
    virtual ~CompatSetting() = default;
    T get() { return SettingsFramework::Setting<T, userScope>::value_.value; }
};

class MockSettingsManager : public SettingsFramework::SettingsManager {
  public:
    virtual SettingsFramework::SettingsHandle attachSetting(
            const SettingId& name, SettingsFramework::UserScope u,
            std::function<void(const std::string&, SettingsFramework::ProfileIdentifier)> onSettingChanged,
            std::function<void(SettingsFramework::ProfileIdentifier p)> onSettingReset) override;
    virtual void detachSetting(const SettingId& name, SettingsFramework::SettingsHandle handle) override;
    virtual std::string getRawData(const SettingId& name, SettingsFramework::ProfileIdentifier profid) override;
    virtual void setRawData(const SettingId& name, SettingsFramework::ProfileIdentifier profid,
                            const std::string& data) override;

  private:
    std::map<SettingId, std::string> storage;
    std::map<SettingId, std::function<void(const std::string&, SettingsFramework::ProfileIdentifier)>> callbacks;
};
