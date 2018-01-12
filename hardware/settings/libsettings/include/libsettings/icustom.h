/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <json.hpp>

namespace SettingsFramework {

// Implement this interface when you want to create your own serializable classes to be stored persistently in settings
class ICustomSetting {
  public:
    virtual ~ICustomSetting() = default;
    virtual void decode(const nlohmann::json& j) = 0;
    virtual nlohmann::json encode() const = 0;
    virtual bool isEqualTo(const ICustomSetting& other) const = 0;
};

inline bool operator!=(const ICustomSetting& lhs, const ICustomSetting& rhs) { return !lhs.isEqualTo(rhs); }

}  // namespace SettingsFramework
