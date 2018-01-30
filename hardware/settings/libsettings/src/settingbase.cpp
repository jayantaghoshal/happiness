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

SettingBase::SettingBase(const android::sp<SettingsManager>& context, const SettingId& name, UserScope userScope)
    : userScope_(userScope), name_(name), context{context}, handle_(-1) {
    ALOGV("SettingBase ctor %d, userScope=%d", name, userScope);
    handle_ = context->attachSetting(
            name_, userScope,
            [this](const std::string& stringdata, ProfileIdentifier profileId) {
                ALOGV("onDataChangedBase, data=%s", stringdata.c_str());
                initialized = true;
                onDataChanged(stringdata, profileId);  // NOLINT: Virtual function will exist on dispatcher thread
                ALOGV("onDataChangedBaseDone");
            },
            [this](ProfileIdentifier profileId) {
                ALOGV("onSettingResetBase");
                initialized = true;
                onSettingReset(profileId);  // NOLINT: Virtual function will exist on dispatcher thread
                ALOGV("onSettingResetBaseDone");
            });
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
    assertInitialized();

    if (userScope_ == UserScope::NOT_USER_RELATED && profid != ProfileIdentifier::None) {
        throw std::runtime_error("Attempt to setStringData for profile on non-profile releated setting, name=" +
                                 toString(name_) + "profile=" + std::to_string(static_cast<int>(profid)));
    }

    context->setRawData(name_, profid, data);
}

std::string SettingBase::jsonToVec(const nlohmann::json& j) { return j.dump(); }

}  // namespace SettingsFramework
