/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

namespace SettingsFramework {

using SettingsHandle = int;

enum class UserScope {
    // The setting is not connected to a particular user profile but globally available for all profiles.
    NOT_USER_RELATED,

    // The setting exists as a separate value for each user profile.
    USER,
};

enum class ProfileIdentifier {
    None = 0,
    Profile1 = 1,
    Profile2 = 2,
    Profile3 = 3,
    Profile4 = 4,
    Profile5 = 5,
    Profile6 = 6,
    Profile7 = 7,
    Profile8 = 8,
    Profile9 = 9,
    Profile10 = 10,
    Profile11 = 11,
    Profile12 = 12,
    Guest = 13,
};
}  // namespace SettingsFramework