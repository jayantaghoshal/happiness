/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "libsettings/settingsmanagerhidl.h"

namespace InterfaceMapping {

class ProfileIdentifier {
  public:
    static autosar::IdPen SettingsFramework_To_Flexray(SettingsFramework::ProfileIdentifier value);
    static SettingsFramework::ProfileIdentifier Flexray_To_SettingsFramework(autosar::IdPen value);
};
}
