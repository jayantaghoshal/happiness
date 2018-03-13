/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <utils/RefBase.h>
#include <chrono>
#include <json.hpp>
#include <string>
#include <type_traits>
#include "libsettings/icustom.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanager.h"
#include "libsettings/settingstypes.h"

namespace SettingsFramework {

class SettingBase {
  public:
    SettingBase(android::sp<SettingsManager> context, const SettingId& name, UserScope userScope);
    virtual ~SettingBase();
    SettingBase(const SettingBase&) = delete;
    SettingBase(SettingBase&&) = delete;
    SettingBase& operator=(const SettingBase&) = delete;
    SettingBase& operator=(SettingBase&&) = delete;

    void setCallback(std::function<void()>&& settingChangedCallback);

  protected:
    void assertInitialized() const;
    std::string getStringData(ProfileIdentifier profid) const;
    void setStringData(const std::string& data, ProfileIdentifier profid);

    static std::string jsonToVec(const nlohmann::json& j);

    const UserScope userScope_;
    bool initialized = false;

  public:
    const SettingId name_;

  private:
    virtual void onDataChanged(const std::string& stringData, ProfileIdentifier profileId) = 0;
    virtual void onSettingReset(ProfileIdentifier profileId) = 0;

    const android::sp<SettingsManager> context;

  protected:
    SettingsHandle handle_;
};

// =====================================================
// Helper types and functions
template <typename T>
using CustomJsonDecodeType = typename std::enable_if<std::is_base_of<ICustomSetting, T>::value, T>::type;
template <typename T>
using EnumType = typename std::enable_if<std::is_enum<T>::value, T>::type;
// For std::chrono::duration
template <typename T>
struct is_duration : std::false_type {};
template <typename Rep, typename Period>
struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};
template <typename T>
using DurationType = typename std::enable_if<is_duration<T>::value, T>::type;
// For std::chrono::time_point
template <typename T>
struct is_time_point : std::false_type {};
template <typename Clock, typename Duration>
struct is_time_point<std::chrono::time_point<Clock, Duration>> : std::true_type {};
template <typename T>
using TimePointType = typename std::enable_if<is_time_point<T>::value, T>::type;

template <typename T>
using JsonDecodeType =
        typename std::enable_if<(!std::is_base_of<ICustomSetting, T>::value) && (!std::is_enum<T>::value) &&
                                        (!is_duration<T>::value) &&
                                        (!is_time_point<T>::value),
                                T>::type;

template <typename T>
CustomJsonDecodeType<T> decodeFromJson(const nlohmann::json& j) {
    T result;
    result.decode(j);
    return result;
}

template <typename T>
JsonDecodeType<T> decodeFromJson(const nlohmann::json& j) {
    return j.get<T>();
}

template <typename T>
EnumType<T> decodeFromJson(const nlohmann::json& j) {
    return static_cast<T>(j.get<typename std::underlying_type<T>::type>());
}

template <typename T>
DurationType<T> decodeFromJson(const nlohmann::json& j) {
    return DurationType<T>{j.get<typename DurationType<T>::rep>()};
}

template <typename T>
TimePointType<T> decodeFromJson(const nlohmann::json& j) {
    using Duration = typename TimePointType<T>::duration;

    Duration const duration{j.get<typename Duration::rep>()};
    return TimePointType<T>{duration};
}

template <typename T>
nlohmann::json encodeToJson(const CustomJsonDecodeType<T>& v) {
    return v.encode();
}

template <typename T>
nlohmann::json encodeToJson(const JsonDecodeType<T>& v) {
    return nlohmann::json(v);
}

template <typename T>
nlohmann::json encodeToJson(const EnumType<T>& v) {
    return nlohmann::json(static_cast<typename std::underlying_type<T>::type>(v));
}

template <typename T>
nlohmann::json encodeToJson(const DurationType<T>& v) {
    return nlohmann::json(v.count());
}

template <typename T>
nlohmann::json encodeToJson(const TimePointType<T>& v) {
    return nlohmann::json(v.time_since_epoch().count());
}

}  // namespace SettingsFramework