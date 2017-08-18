/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus_dbus_common.h"

const std::string DECommonDBUS::DBUS_INTERFACE_NAME("com.ihu.VehicleSignalsManager");
const std::string DECommonDBUS::DBUS_OBJECT_NAME("/com/ihu/VehicleSignalsManager");

const std::string DECommonDBUS::DBUS_INTERFACE_NAME_IN(DBUS_INTERFACE_NAME + ".In");
const std::string DECommonDBUS::DBUS_INTERFACE_NAME_OUT(DBUS_INTERFACE_NAME + ".Out");
const std::string DECommonDBUS::DBUS_INTERFACE_NAME_INTERNAL(DBUS_INTERFACE_NAME + ".Internal");
const std::string DECommonDBUS::DBUS_OBJECT_NAME_IN(DBUS_OBJECT_NAME + "/In");
const std::string DECommonDBUS::DBUS_OBJECT_NAME_OUT(DBUS_OBJECT_NAME + "/Out");
const std::string DECommonDBUS::DBUS_OBJECT_NAME_INTERNAL(DBUS_OBJECT_NAME + "/Internal");
const std::string DECommonDBUS::DBUS_SIGNALS_CHANGED_NAME("DESignalChanged");

const std::string& DECommonDBUS::dirToDBUSInterfaceName(autosar::Dir dir)
{
    switch (dir)
    {
    case autosar::Dir::IN:
        return DBUS_INTERFACE_NAME_IN;
    case autosar::Dir::OUT:
        return DBUS_INTERFACE_NAME_OUT;
    case autosar::Dir::INTERNAL:
        return DBUS_INTERFACE_NAME_INTERNAL;
    }

    return DBUS_INTERFACE_NAME_IN;
}
const std::string& DECommonDBUS::dirToDBUSObjectName(autosar::Dir dir)
{
    switch (dir)
    {
    case autosar::Dir::IN:
        return DBUS_OBJECT_NAME_IN;
    case autosar::Dir::OUT:
        return DBUS_OBJECT_NAME_OUT;
    case autosar::Dir::INTERNAL:
        return DBUS_OBJECT_NAME_INTERNAL;
    }

    return DBUS_OBJECT_NAME_IN;
}
