/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <utils/RefBase.h>
#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include "libsettings/icustom.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingstypes.h"

namespace SettingsFramework {

class SettingsManager : public virtual android::RefBase {
  public:
    virtual ~SettingsManager() = default;

    virtual SettingsHandle attachSetting(const SettingId& name,
                                         UserScope u,
                                         std::function<void(const std::string&, ProfileIdentifier)> onSettingChanged,
                                         std::function<void(ProfileIdentifier p)> onSettingReset) = 0;
    virtual void detachSetting(const SettingId& name, SettingsHandle handle) = 0;

    virtual std::string getRawData(const SettingId& name, ProfileIdentifier profid) = 0;
    virtual void setRawData(const SettingId& name, ProfileIdentifier profid, const std::string& data) = 0;
};
}  // namespace SettingsFramework