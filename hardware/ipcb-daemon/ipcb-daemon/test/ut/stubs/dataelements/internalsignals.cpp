/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "internalsignals.h"

// ==============================================================
// ActivateVfc
const char* ActivateVfc_info::Name() { return "ActivateVfc"; }
const char* ActivateVfc_info::TypeName() { return "ActivateVfc"; }
autosar::Dir ActivateVfc_info::Direction() { return autosar::Dir::OUT; }
	
const char* ActivateVfc_info::name() const { return Name(); }
const char* ActivateVfc_info::typeName() const { return TypeName(); }
autosar::Dir ActivateVfc_info::direction() const { return Direction(); }

namespace autosar {
template <>
QJsonValue toJson<ActivateVfc>(const ActivateVfc& v) {
    QJsonObject j;
    j["vfc"] = toJson<Vfc>(v.vfcToActivate);
    j["sec"] = toJson<uint8_t>(v.secondsToKeepActive);
    return QJsonValue(j);
}
template <>
ActivateVfc fromJson<ActivateVfc>(const QJsonValue& j) {
    if (!j.isObject()) {
        return ActivateVfc();
    }
    QJsonObject object = j.toObject();
    ActivateVfc v;
    v.vfcToActivate = fromJson<Vfc>(object["vfc"]);
    v.secondsToKeepActive = fromJson<uint8_t>(object["sec"]);
    return v;
}
} // end namespace
