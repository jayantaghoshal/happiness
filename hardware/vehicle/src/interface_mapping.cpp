/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "interface_mapping.h"

namespace InterfaceMapping {

static const std::map<SettingsFramework::ProfileIdentifier, autosar::IdPen> UserProfileMap{
        {SettingsFramework::ProfileIdentifier::None, autosar::IdPen::ProfUkwn},
        {SettingsFramework::ProfileIdentifier::Profile1, autosar::IdPen::Prof1},
        {SettingsFramework::ProfileIdentifier::Profile2, autosar::IdPen::Prof2},
        {SettingsFramework::ProfileIdentifier::Profile3, autosar::IdPen::Prof3},
        {SettingsFramework::ProfileIdentifier::Profile4, autosar::IdPen::Prof4},
        {SettingsFramework::ProfileIdentifier::Profile5, autosar::IdPen::Prof5},
        {SettingsFramework::ProfileIdentifier::Profile6, autosar::IdPen::Prof6},
        {SettingsFramework::ProfileIdentifier::Profile7, autosar::IdPen::Prof7},
        {SettingsFramework::ProfileIdentifier::Profile8, autosar::IdPen::Prof8},
        {SettingsFramework::ProfileIdentifier::Profile9, autosar::IdPen::Prof9},
        {SettingsFramework::ProfileIdentifier::Profile10, autosar::IdPen::Prof10},
        {SettingsFramework::ProfileIdentifier::Profile11, autosar::IdPen::Prof11},
        {SettingsFramework::ProfileIdentifier::Profile12, autosar::IdPen::Prof12},
        {SettingsFramework::ProfileIdentifier::Guest, autosar::IdPen::Prof13}
        // Other IdPen values not used in ProfileIdentifier
        // Resd14 = 14,
        // ProfAll = 15,
};

autosar::IdPen ProfileIdentifier::SettingsFramework_To_Flexray(SettingsFramework::ProfileIdentifier value) {
    auto it = UserProfileMap.find(value);
    if (it != UserProfileMap.end()) {
        return it->second;
    }

    throw std::out_of_range("No matching element found.");
}

SettingsFramework::ProfileIdentifier ProfileIdentifier::Flexray_To_SettingsFramework(autosar::IdPen value) {
    for (auto it = UserProfileMap.begin(); it != UserProfileMap.end(); ++it) {
        if (it->second == value) {
            return it->first;
        }
    }

    throw std::out_of_range("No matching element found.");
}
}