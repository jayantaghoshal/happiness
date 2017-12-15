/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef _INTERNALSIGNALS_H
#define _INTERNALSIGNALS_H

#include "gen_dataelements.h"
#include "gen_jsonencdec.h"

/*****************************************************************
 * Declare internal dataelements, types and serializers here.    *
 * Internal means it is a dataelement/signal that is ONLY        *
 * sent and received between software modules within the IHU.    *
 *****************************************************************/

// ==============================================================
// ActivateVfc
enum class Vfc {
    AdjustableDriveControls = 0,
    AllECUs = 1,
    Crash = 2,
    Diagnostics = 3,
    DriverInformationCenterStackDisplayPoll = 4,
    DriverInformationCenterStackDisplayPush = 5,
    DriverInformationClusterPush = 6,
    DrivingClimatisation = 7,
    Entertainment = 8,
    Entrance = 9,
    GlobalShortDuration = 10,
    InteriorLightingDay = 11,
    InteriorLightingEntrance = 12,
    InteriorLightingNight = 13,
    IPWakeup = 14,
    Locking = 15,
    PanicAlarm = 16,
    ParkingClimatisationLV = 17,
    PostClimatisation = 18,
    PreClimatisationHV = 19,
    SeatComfortFunctions = 20,
    TelematicsConnectivity = 21,
    TrailerCaravanFunctions = 22,
    UserInputSettings = 23,
    UserProfileInput = 24,
    WarningSounds = 25,
    VehicleDriving = 26,
    VehicleDrivingLowSpeed = 27,
    Windows = 28,
    Visibility = 29,
    PowerOperatedTailgate = 30
};

struct ActivateVfc {
    Vfc vfcToActivate;
    uint8_t secondsToKeepActive;  // max 60secs
};

struct ActivateVfc_info : autosar::OutTag, public autosar::DataElemInfo {
    using data_elem_type = ActivateVfc;

    static const char* Name();
    static const char* TypeName();
    static autosar::Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    autosar::Dir direction() const override;
};

namespace autosar {
template <>
json toJson<ActivateVfc>(const ActivateVfc& v);
template <>
ActivateVfc fromJson<ActivateVfc>(const json& j);
}

#endif
