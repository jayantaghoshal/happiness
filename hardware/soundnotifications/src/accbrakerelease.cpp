/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <cutils/log.h>

#include "accbrakerelease.h"

using namespace ApplicationDataElement;
using namespace autosar;

#undef LOG_TAG
#define LOG_TAG "SoundNotifyACCBrkRel"

namespace SoundNotifications {
ACCBrakeReleaseWarning::ACCBrakeReleaseWarning()
    : lastPlayedSound(AudioTable::SoundType::LastSound, AudioTable::SoundComponent::NotAvailable, false) {
    ALOGI("ACCBrakeReleaseWarning::ACCBrakeReleaseWarning()");

    BrkRelsWarnReq.subscribe([this] { InjectSignals(); });
}

void ACCBrakeReleaseWarning::InjectSignals() {
    /* REQPROD:281557/MAIN;3 Audible Brake Release Warning */

    const auto signal = BrkRelsWarnReq.get();
    bool playCondition = ((signal.isOk()) && (signal.value() == autosar::NoYes1::Yes));
    ALOGI("ACCBrakeReleaseWarning::injectSignals() %i", playCondition);
    if (playCondition) {
        lastPlayedSound = SoundWrapper::SoundID(AudioTable::SoundType::ACCBrakeReleaseWarning,
                                                AudioTable::SoundComponent::NotAvailable);
        SoundWrapper::play(lastPlayedSound);
    } else {
        if (lastPlayedSound.isValid) {
            SoundWrapper::stop(lastPlayedSound);
        }
    }
}
}  // namespace SoundNotifications
