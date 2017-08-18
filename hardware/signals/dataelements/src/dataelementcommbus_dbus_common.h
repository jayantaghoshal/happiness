/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include <string>
#include "gen_dataelements.h"

class DECommonDBUS {
 public:
  static const std::string DBUS_INTERFACE_NAME;
  static const std::string DBUS_OBJECT_NAME;

  static const std::string DBUS_INTERFACE_NAME_IN;
  static const std::string DBUS_INTERFACE_NAME_OUT;
  static const std::string DBUS_INTERFACE_NAME_INTERNAL;

  static const std::string DBUS_OBJECT_NAME_IN;
  static const std::string DBUS_OBJECT_NAME_OUT;
  static const std::string DBUS_OBJECT_NAME_INTERNAL;

  static const std::string DBUS_SIGNALS_CHANGED_NAME;

  static const std::string& dirToDBUSInterfaceName(autosar::Dir dir);
  static const std::string& dirToDBUSObjectName(autosar::Dir dir);
};
