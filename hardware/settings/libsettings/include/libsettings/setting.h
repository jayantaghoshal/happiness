/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <cutils/log.h>
#include <json.hpp>
#include "libsettings/settingbase.h"
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

template <typename T, UserScope userScope = UserScope::NOT_USER_RELATED>
class Setting : public SettingBase {
  public:
    Setting(const std::string& name, const T& defaultValue, std::shared_ptr<SettingsContext> context)
        : SettingBase(context, name, userScope), value_(defaultValue), default_(defaultValue) {}

    /*
     * Get value for the setting.
     * NOTE: This function may not be called until setCallback-notification has been called.
     *       Such notification indicates that settings-backend is connected
     *
     * Depending on if the setting is USER_RELATED or not this method behaves differently:
     * USER_RELATED: get value for active profile
     * NOT_USER_RELATED: get value (for the car globally)
     */
    T get() const {
        assertInitialized();
        return getValueAndProfile().value;
    }

    T defaultValue() const { return default_; }

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
        return parse(stringdata);
    }

    /*
     * Get a <value,profileId> tuple for the Settings instance. The returned .profileId is, at the time of the call,
     * the currently active profile. The .value is the value that belongs to that profile.
     * The returned values are GUARANTEED to be consistent meaning that the .value is really the value for the
     * .profileId
     * Only use this method if you want to get a value that is user-related!!
     *
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * IMPORTANT
     * If you have a use case like the following in your code:
     * 1) get user specific data from settings
     * 2) modify that data and then
     * 3) set it again in settings
     * then you SHOULD use getValueAndProfile()() and setForProfile() to correctly handle
     * that a profile change happens after 1) but before 3) above.
     */
    ValueProfile<T> getValueAndProfile() const {
        assertInitialized();
        return value_;
    }

    /*
     * Set the value (.value) for the provided profile (.profileId)
     * Only use this method if you want to set a value for a
     * specific profile (including possibly the active profile).
     */
    void setForProfile(const ValueProfile<T>& newValue) {
        ALOG(LOG_DEBUG, SETTINGS_LOG_TAG, "setForProfile, name=%s, profileid=%d", name().c_str(), newValue.profileId);
        assertInitialized();
        if (userScope == UserScope::NOT_USER_RELATED || newValue.profileId == value_.profileId ||
            newValue.profileId == ProfileIdentifier::None) {
            const bool dirty = (value_.value != newValue.value);
            value_ = newValue;
            const nlohmann::json j(encodeToJson<T>(newValue.value));
            setStringData(j.dump(), newValue.profileId);

            if (dirty) {
                if (callbackToApplicationOnSettingChanged_) {
                    callbackToApplicationOnSettingChanged_();
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
        nlohmann::json j = nlohmann::json::parse(stringdata);
        T valueOut = decodeFromJson<T>(j);
        return valueOut;
    }

    void onSettingReset(ProfileIdentifier profileId) override {
        value_ = ValueProfile<T>(default_, profileId);
        ALOG(LOG_VERBOSE, SETTINGS_LOG_TAG, "Setting::onSettingReset");
        if (callbackToApplicationOnSettingChanged_) {
            callbackToApplicationOnSettingChanged_();
        }
    }

    void onDataChanged(const std::string& stringData, ProfileIdentifier profileId) override {
        ALOG(LOG_VERBOSE, SETTINGS_LOG_TAG, "OnDataChanged, name=%s, value=%s, profileid=%d", name().c_str(),
             stringData.c_str(), profileId);
        if (userScope == UserScope::NOT_USER_RELATED) {
            profileId = ProfileIdentifier::None;
        }

        try {
            T v = parse(stringData);
            value_ = ValueProfile<T>(v, profileId);
        } catch (const std::exception& e) {
            ALOG(LOG_ERROR, SETTINGS_LOG_TAG,
                 "Failed to parse stored setting. Fallback to default. Key=%s, Value=%s, Error=%s", name().c_str(),
                 stringData.c_str(), e.what());
            value_ = ValueProfile<T>(default_, profileId);
        }
        if (callbackToApplicationOnSettingChanged_) {
            callbackToApplicationOnSettingChanged_();
        }
    }

  private:
    mutable ValueProfile<T> value_;
    const T default_;
};
}  // namespace SettingsFramework
