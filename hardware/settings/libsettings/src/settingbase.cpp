/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "libsettings/settingbase.h"
#include "libsettings/settingsmanager.h"

#include <IDispatcher.h>
#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "Settings"

namespace SettingsFramework {

namespace {
std::string toString(SettingId id) { return std::to_string(static_cast<uint32_t>(id)); }
}  // namespace

SettingBase::SettingBase(android::sp<SettingsManager> context, const SettingId& name, UserScope userScope)
    : userScope_(userScope), name_(name), context{std::move(context)}, handle_(-1) {
    ALOGV("SettingBase ctor %d, userScope=%d", name, userScope);

    ALOGV("SettingBase ctor done");
}

SettingBase::~SettingBase() {
    ALOGV("SettingBase destructor");
    if (handle_ > 0) {
        context->detachSetting(name_, handle_);
    }
    ALOGV("SettingBase destructor detached");
}

void SettingBase::assertInitialized() const {
    if (!initialized) {
        throw std::runtime_error("Settings is not initialized, name=" + toString(name_));
    }
}

std::string SettingBase::getStringData(ProfileIdentifier profid) const {
    assertInitialized();

    if (userScope_ == UserScope::NOT_USER_RELATED && profid != ProfileIdentifier::None) {
        throw std::runtime_error("Attempt to getStringData for profile on non-profile releated setting, name=" +
                                 toString(name_) + "profile=" + std::to_string(static_cast<int>(profid)));
    }

    return context->getRawData(name_, profid);
}

void SettingBase::setStringData(const std::string& data, ProfileIdentifier profid) {
    // assertInitialized();
    if (!initialized) {
        // If you end up here you most likely have a initialization order bug in your application code.
        // It means you try to store a setting before reading/receiving the previously stored value. Doing so
        // kindof defeats the purpose of storing the value persistently.
        //
        // If the set-request was triggered by a user action it is probably more correct to use the new value but we've
        // observed multiple
        // cases of application code incorrectly calling set during startup which would overwrite stored settings so
        // we've been forced
        // to choose ignore as default.
        ALOGE("Attempt to set setting [%d] before stored value has been received. This is most likely a bug in "
              "application code. Set request will be ignored.",
              name_);
        return;
    }

    if (userScope_ == UserScope::NOT_USER_RELATED && profid != ProfileIdentifier::None) {
        throw std::runtime_error("Attempt to setStringData for profile on non-profile releated setting, name=" +
                                 toString(name_) + "profile=" + std::to_string(static_cast<int>(profid)));
    }

    context->setRawData(name_, profid, data);
}

std::string SettingBase::jsonToVec(const nlohmann::json& j) { return j.dump(); }

}  // namespace SettingsFramework
