/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <cutils/log.h>

#include "parking_brake.h"
#include "soundwrapper.h"

using namespace ApplicationDataElement;
using namespace autosar;

#undef LOG_TAG
#define LOG_TAG "SoundNotifyPrkngBrk"

/*  Here the actual speed value is in m/s. As per requirement:
 *  it should be in Km/hr
 *  km/hr to m/s conversion:
 *  Minimum speed value is 5  km/hr: 5/3.6  = 1.38805 m/s (approx)
 *  Maximum speed value is 10 km/hr: 10/3.6 = 2.7761 m/s (approx)
 */
#define MINSPEED 1.38805
#define MAXSPEED 2.7761

SoundNotifications::Chassis::ParkingBrake::ParkingBrake() : state(EPBState::Idle) {
    VehSpdLgtSafe.subscribe([this]() { OnSignalChange(); });
    EpbLampReq.subscribe([this]() { OnSignalChange(); });
}

/***************************************************************************************************************
 * REQPROD:218654/MAIN;3 Audio request for EPB warning
 ***************************************************************************************************************/

void SoundNotifications::Chassis::ParkingBrake::OnSignalChange() {
    SoundWrapper::Result result = SoundWrapper::Result::INVALID_STATE;

    switch (state) {
        case EPBState::Idle:
            if (VehSpdLgtSafe.get().isOk() && EpbLampReq.get().isOk() &&
                VehSpdLgtSafe.get().value().VehSpdLgt > MAXSPEED &&
                (EpbLampReq.get().value().EpbLampReq == autosar::EpbLampReqType1::On ||
                 EpbLampReq.get().value().EpbLampReq == autosar::EpbLampReqType1::Flash3)) {
                state = EPBState::Active;
                ALOGD("ActiveSafetyWarnings: play EPBLampRequestWarning");
                result = SoundWrapper::play({AudioTable::SoundType::ElectricalParkingBrakeEPB,
                                             AudioTable::SoundComponent::NotAvailable});  // this will/should play for
                                                                                          // ever, see sound matrix
            }
            break;
        case EPBState::Active:
            if (VehSpdLgtSafe.get().isError() || EpbLampReq.get().isError() ||
                (EpbLampReq.get().value().EpbLampReq != autosar::EpbLampReqType1::On &&
                 EpbLampReq.get().value().EpbLampReq != autosar::EpbLampReqType1::Flash3) ||
                VehSpdLgtSafe.get().value().VehSpdLgt <= MINSPEED) {
                state = EPBState::Idle;
                ALOGD("ActiveSafetyWarnings: stop EPBLampRequestWarning");
                result = SoundWrapper::stop(
                        {AudioTable::SoundType::ElectricalParkingBrakeEPB, AudioTable::SoundComponent::NotAvailable});
            }
            break;
    }

    if (result == SoundWrapper::Result::OK) {
        ALOGV("Play parking brake sound OK");
    } else if (result == SoundWrapper::Result::PLAY_FAILED) {
        ALOGW("Fail play parking brake sound");
    }
}
