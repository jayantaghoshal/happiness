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

SettingsContext::SettingsContext(tarmac::eventloop::IDispatcher& dispatcher, android::sp<SettingsManager> manager)
    : dispatcher{dispatcher},
      manager{std::move(manager)} {

      };

SettingBase::SettingBase(const std::shared_ptr<SettingsContext>& context, const std::string& name, UserScope userScope)
    : callbackToApplicationOnSettingChanged_(nullptr),
      userScope_(userScope),
      context{context},
      handle_(-1),
      name_(name) {
    ALOGV("SettingBase ctor %s", name.c_str());
    if (name == "") {
        throw std::runtime_error("Setting with empty name");
    }
    std::weak_ptr<SettingsContext> contextWeak = context;
    handle_ = context->manager->attachSetting(
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
        context->manager->detachSetting(name_, handle_);
    }
    ALOGV("SettingBase destructor detached");
}

std::string SettingBase::name() const { return name_; }

void SettingBase::assertInitialized() const {
    if (!initialized) {
        throw std::runtime_error("Settings is not initialized, name=" + name_);
    }
}

std::string SettingBase::getStringData(ProfileIdentifier profid) const {
    assertInitialized();

    if (userScope_ == UserScope::NOT_USER_RELATED && profid != ProfileIdentifier::None) {
        throw std::runtime_error("Attempt to getStringData for profile on non-profile releated setting, name=" + name_ +
                                 "profile=" + std::to_string(static_cast<int>(profid)));
    }

    return context->manager->getRawData(name(), profid);
}

void SettingBase::setStringData(const std::string& data, ProfileIdentifier profid) {
    assertInitialized();

    if (userScope_ == UserScope::NOT_USER_RELATED && profid != ProfileIdentifier::None) {
        throw std::runtime_error("Attempt to setStringData for profile on non-profile releated setting, name=" + name_ +
                                 "profile=" + std::to_string(static_cast<int>(profid)));
    }

    context->manager->setRawData(name_, profid, data);
}

void SettingBase::setCallback(std::function<void()>&& settingChangedCallback) {
    ALOGV("setCallback, name=%s", name_.c_str());
    callbackToApplicationOnSettingChanged_ = std::move(settingChangedCallback);
    if (initialized && callbackToApplicationOnSettingChanged_) {
        callbackToApplicationOnSettingChanged_();
    }
}

std::string SettingBase::jsonToVec(const nlohmann::json& j) { return j.dump(); }

}  // namespace SettingsFramework
