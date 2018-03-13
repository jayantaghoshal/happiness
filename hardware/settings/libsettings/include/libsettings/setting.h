/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <cutils/log.h>
#include <json.hpp>
#include "libsettings/settingbase.h"
#include "libsettings/settingsmanager.h"
#include "libsettings/settingstypes.h"

#define SETTINGS_LOG_TAG "SETTINGS"

namespace SettingsFramework {

template <typename T>
struct ValueProfile {
    ValueProfile(const T& v) : value(v), profileId(ProfileIdentifier::None) {}
    ValueProfile(const T& v, ProfileIdentifier p) : value(v), profileId(p) {}

    T value;
    ProfileIdentifier profileId;
};

template <typename T, UserScope userScope>
class Setting : public SettingBase {
  public:
    Setting(const SettingId& name, const T& defaultValue, android::sp<SettingsManager> context)
        : SettingBase(context, name, userScope), value_(defaultValue), default_(defaultValue) {
        handle_ = context->attachSetting(name_,
                                         userScope,
                                         [this](const std::string& stringdata, ProfileIdentifier profileId) {
                                             initialized = true;
                                             onDataChanged(stringdata, profileId);
                                         },
                                         [this](ProfileIdentifier profileId) {
                                             initialized = true;
                                             onSettingReset(profileId);
                                         });
    }

    void setCallback(std::function<void(const ValueProfile<T>&)>&& settingChangedCallback) {
        callbackToApplicationOnSettingChanged_ = std::move(settingChangedCallback);
        if (initialized && callbackToApplicationOnSettingChanged_) {
            callbackToApplicationOnSettingChanged_(value_);
        }
    }

    virtual T defaultValue() const { return default_; }

    /*
     * Get value for the given profile.
     * DONT use for NOT_USER_RELATED
     */
    virtual T getForProfile(ProfileIdentifier profileId) const {
        assertInitialized();

        if (profileId == value_.profileId || profileId == ProfileIdentifier::None) {
            return value_.value;
        }

        const auto stringdata = getStringData(profileId);
        if (stringdata == "") {
            ALOG(LOG_INFO,
                 SETTINGS_LOG_TAG,
                 "%s:getStringData returned empty string. Fallback to default value",
                 __FUNCTION__);
            return default_;
        }
        return parse(stringdata);
    }

    /*
     * Set the value (.value) for the provided profile (.profileId)
     * Only use this method if you want to set a value for a
     * specific profile (including possibly the active profile).
     *
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * IMPORTANT
     * If you have a use case like the following in your code:
     * 1) get user specific data from settings
     * 2) modify that data and then (eg increment a counter)
     * 3) set it again in settings
     * then you SHOULD use getValueAndProfile()() and setForProfile() to correctly handle
     * that a profile change happens after 1) but before 3) above.
     */
    void setForProfile(const ValueProfile<T>& newValue) {
        ALOG(LOG_DEBUG, SETTINGS_LOG_TAG, "setForProfile, name=%d, profileid=%d", name_, newValue.profileId);
        assertInitialized();
        if (userScope == UserScope::NOT_USER_RELATED || newValue.profileId == value_.profileId ||
            newValue.profileId == ProfileIdentifier::None) {
            const bool dirty = (value_.value != newValue.value);

            if (dirty) {
                value_ = newValue;
                {
                    // NOTE: Only notifying server in case client thinks setting is dirty,
                    //      requires client to be fully synchronized with server.
                    //      This should always be the case as we only allow one client per setting.
                    const nlohmann::json j(encodeToJson<T>(newValue.value));
                    setStringData(j.dump(), newValue.profileId);
                }

                if (callbackToApplicationOnSettingChanged_) {
                    callbackToApplicationOnSettingChanged_(value_);
                }
            }
        } else {
            const nlohmann::json j(encodeToJson<T>(newValue.value));
            setStringData(j.dump(), newValue.profileId);
        }
    }

    /*
     * Set value for the setting. Depending on if the setting is USER_RELATED or not this method behaves differently:
     * USER_RELATED: set value for active profile
     * NOT_USER_RELATED: set value (for the car globally)
     */
    void set(const T& v) { setForProfile(ValueProfile<T>(v, value_.profileId)); }

    // Only use this method if you want to set a value for a specific profile
    void setForProfile(const T& v, ProfileIdentifier profid) { setForProfile(ValueProfile<T>(v, profid)); }

  protected:
    /*
     * NOTE: This function may throw exceptions
     */
    T parse(const std::string& stringdata) const {
        ALOG(LOG_VERBOSE, SETTINGS_LOG_TAG, "Setting::Parse: %s", stringdata.c_str());
        nlohmann::json j = nlohmann::json::parse(stringdata);
        T valueOut = decodeFromJson<T>(j);
        return valueOut;
    }

    void onSettingReset(ProfileIdentifier profileId) override {
        value_ = ValueProfile<T>(default_, profileId);
        ALOG(LOG_VERBOSE, SETTINGS_LOG_TAG, "Setting::onSettingReset");
        if (callbackToApplicationOnSettingChanged_) {
            callbackToApplicationOnSettingChanged_(value_);
        }
    }

    void onDataChanged(const std::string& stringData, ProfileIdentifier profileId) override {
        ALOG(LOG_VERBOSE,
             SETTINGS_LOG_TAG,
             "OnDataChanged, name=%d, value=%s, profileid=%d",
             name_,
             stringData.c_str(),
             profileId);
        if (userScope == UserScope::NOT_USER_RELATED) {
            profileId = ProfileIdentifier::None;
        }

        try {
            T v = parse(stringData);
            value_ = ValueProfile<T>(v, profileId);
        } catch (const std::exception& e) {
            ALOG(LOG_ERROR,
                 SETTINGS_LOG_TAG,
                 "Failed to parse stored setting. Fallback to default. Key=%d, Value=%s, Error=%s",
                 name_,
                 stringData.c_str(),
                 e.what());
            value_ = ValueProfile<T>(default_, profileId);
        }
        if (callbackToApplicationOnSettingChanged_) {
            callbackToApplicationOnSettingChanged_(value_);
        }
    }

  private:
    std::function<void(const ValueProfile<T>&)> callbackToApplicationOnSettingChanged_;

  protected:
    mutable ValueProfile<T> value_;

  private:
    const T default_;
};
}  // namespace SettingsFramework
