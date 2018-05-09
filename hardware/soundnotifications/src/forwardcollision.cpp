/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <cutils/log.h>

#include "forwardcollision.h"
#include "soundwrapper.h"

using namespace ApplicationDataElement;
using namespace autosar;

#undef LOG_TAG
#define LOG_TAG "SoundNotifyFwdCllsn"

namespace SoundNotifications {
ForwardCollision::ForwardCollision()
    : CllsnWarnSoundId(SoundWrapper::SoundID(AudioTable::SoundType::ForwardCollisionWarningACCBrakeCapacity,
                                             AudioTable::SoundComponent::NotAvailable)) {
    ALOGI("Initialized ForwardCollision()");
    CllsnWarnReq.subscribe([this]() { OnSignalChange_CllsnWarnReq(); });
    CllsnFwdWarnReq.subscribe([this]() { OnSignalChange_CllsnFwdWarnReq(); });
}

/***************************************************************************************************************
 * REQPROD:229053/MAIN;4	Audible Forward Collision Warning
 ***************************************************************************************************************/

void ForwardCollision::OnSignalChange_CllsnWarnReq() {
    autosar::OnOff1 cllsnWarnReqValue = CllsnWarnReq.get().isOk() ? CllsnWarnReq.get().value() : autosar::OnOff1::Off;
    autosar::OnOff1 cllsnFwdWarnReqValue =
            CllsnFwdWarnReq.get().isOk() ? CllsnFwdWarnReq.get().value() : autosar::OnOff1::Off;

    // only one signal On will trigger the sound play.
    bool playCondition = (cllsnWarnReqValue == autosar::OnOff1::On);

    if (playCondition) {
        ALOGD("play collision Warning sound");
        SoundWrapper::play(CllsnWarnSoundId);
    } else {
        // both the signals value to Off is required as per the requirement
        if ((cllsnFwdWarnReqValue == autosar::OnOff1::Off) && (cllsnWarnReqValue == autosar::OnOff1::Off)) {
            ALOGD("stop collision Warning sound");
            SoundWrapper::stop(CllsnWarnSoundId);
        }
    }
}

void ForwardCollision::OnSignalChange_CllsnFwdWarnReq() {
    autosar::OnOff1 cllsnFwdWarnReqValue =
            CllsnFwdWarnReq.get().isOk() ? CllsnFwdWarnReq.get().value() : autosar::OnOff1::Off;
    autosar::OnOff1 cllsnWarnReqValue = CllsnWarnReq.get().isOk() ? CllsnWarnReq.get().value() : autosar::OnOff1::Off;

    // only one signal On will trigger the sound play.
    bool playCondition = (cllsnFwdWarnReqValue == autosar::OnOff1::On);

    if (playCondition) {
        ALOGD("play Forward collision Warning sound");
        SoundWrapper::play(CllsnWarnSoundId);
    } else {
        // both the signals value to Off is required as per the requirement
        if ((cllsnFwdWarnReqValue == autosar::OnOff1::Off) && (cllsnWarnReqValue == autosar::OnOff1::Off)) {
            ALOGD("stop Forward collision Warning sound");
            SoundWrapper::stop(CllsnWarnSoundId);
        }
    }
}
}  // namespace SoundNotifications