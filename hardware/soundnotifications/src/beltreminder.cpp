/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <cutils/log.h>

#include "beltreminder.h"

using namespace ApplicationDataElement;
using namespace autosar;

#undef LOG_TAG
#define LOG_TAG "SoundNotifyBeltRmndr"

namespace SoundNotifications {
BeltReminder::BeltReminder()
    : lastPlayedSound(AudioTable::SoundType::LastSound, AudioTable::SoundComponent::WarnUS, false) {
    ALOGI("BeltReminder::BeltReminder()");
    bltSnd1Receiver.subscribe([&]() {
        injectSignals(bltSnd1Receiver.get());

    });
    bltSnd2Receiver.subscribe([&]() {
        injectSignals(bltSnd2Receiver.get());

    });
    bltSnd3Receiver.subscribe([&]() {
        injectSignals(bltSnd3Receiver.get());

    });
    bltSnd4Receiver.subscribe([&]() {
        injectSignals(bltSnd4Receiver.get());

    });
}

template <typename S>
void BeltReminder::injectSignals(S bltSnd) {
    // REQPROD:446629/MAIN;0 Sound request for safety belt reminder

    SoundWrapper::SoundID playedSound(AudioTable::SoundType::LastSound, AudioTable::SoundComponent::WarnUS, false);

    if (bltSnd.value() == Trig1::Trig) {
        if (bltSnd.name() == "BltRmnSound1") {
            playedSound =
                    SoundWrapper::SoundID(AudioTable::SoundType::BeltReminder, AudioTable::SoundComponent::WarnUS);
        } else if (bltSnd.name() == "BltRmnSound2") {
            playedSound = SoundWrapper::SoundID(AudioTable::SoundType::BeltReminder, AudioTable::SoundComponent::Warn1);
        } else if (bltSnd.name() == "BltRmnSound3") {
            playedSound =
                    SoundWrapper::SoundID(AudioTable::SoundType::BeltReminder, AudioTable::SoundComponent::WarnFinal);
        } else if (bltSnd.name() == "BltRmnSound4") {
            playedSound =
                    SoundWrapper::SoundID(AudioTable::SoundType::BeltReminder, AudioTable::SoundComponent::WarnRear);
        }
        SoundWrapper::play(playedSound);
        lastPlayedSound = playedSound;
    } else if (bltSnd.value() == Trig1::NoTrig && lastPlayedSound.isValid) {
        SoundWrapper::stop(lastPlayedSound);
    }
}

}  // namespace SoundNotifications
