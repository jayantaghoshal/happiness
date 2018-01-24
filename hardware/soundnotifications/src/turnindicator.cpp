/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <cutils/log.h>

#include "soundwrapper.h"
#include "turnindicator.h"

using namespace ApplicationDataElement;
using namespace autosar;

#undef LOG_TAG
#define LOG_TAG "SoundNotifyTurnInd"

using namespace autosar;

namespace SoundNotifications {
TurnIndicator::TurnIndicator() : previous_IndcrDisp1WdSts(IndcrSts1::Off) {
    indcrRecevier.subscribe([&]() {
        ALOGV("indcr received");

        injectSignals(indcrRecevier.get(), fltIndLeFrontReceiver.get(), fltIndLeReReceiver.get(),
                      fltIndRiFrontReceiver.get(), fltIndRiReReceiver.get());

    });
}

void TurnIndicator::injectSignals(DataElemValue<autosar::IndcrDisp1WdSts_info> indcr,
                                  DataElemValue<autosar::FltIndcrTurnLeFrnt_info> FltIndcrTurnLeFrnt,
                                  DataElemValue<autosar::FltIndcrTurnLeRe_info> FltIndcrTurnLeRe,
                                  DataElemValue<autosar::FltIndcrTurnRiFrnt_info> FltIndcrTurnRiFrnt,
                                  DataElemValue<autosar::FltIndcrTurnRiRe_info> FltIndcrTurnRiRe) {
    // REQPROD:218373/MAIN;5 Audio request for Turn Indicator

    IndcrSts1 from = previous_IndcrDisp1WdSts;
    IndcrSts1 to = indcr.isOk() ? (indcr.value()) : IndcrSts1::Off;

    SoundWrapper::Result result = SoundWrapper::Result::INVALID_STATE;

    if (to == IndcrSts1::LeOn) {
        if (from == IndcrSts1::Off || from == IndcrSts1::RiOn || from == IndcrSts1::LeAndRiOn) {
            if (FltIndcrTurnLeFrnt.isError() || FltIndcrTurnLeRe.isError()) {
                ALOGW("No turnindicator left sound played because FltIndcr signal not valid");
            } else if ((FltIndcrTurnLeFrnt.value() == DevErrSts2::NoFlt) &&
                       (FltIndcrTurnLeRe.value() == DevErrSts2::NoFlt)) {
                result = SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                                  AudioTable::SoundComponent::LeftRight));
            } else {
                result = SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                                  AudioTable::SoundComponent::LeftRightBroken));
            }
        }
    } else if (to == IndcrSts1::RiOn) {
        if (from == IndcrSts1::Off || from == IndcrSts1::LeOn || from == IndcrSts1::LeAndRiOn) {
            if (FltIndcrTurnRiFrnt.isError() || FltIndcrTurnRiRe.isError()) {
                ALOGW("No turnindicator right sound played because FltIndcr signal not valid");
            } else if ((FltIndcrTurnRiFrnt.value() == DevErrSts2::NoFlt) &&
                       (FltIndcrTurnRiRe.value() == DevErrSts2::NoFlt)) {
                result = SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                                  AudioTable::SoundComponent::LeftRight));

            } else {
                result = SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                                  AudioTable::SoundComponent::LeftRightBroken));
            }
        }
    } else if (to == IndcrSts1::LeAndRiOn) {
        if (from == IndcrSts1::Off || from == IndcrSts1::LeOn || from == IndcrSts1::RiOn) {
            result = SoundWrapper::play(
                    SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator, AudioTable::SoundComponent::Hazard));
        }
    } else {
        // off
    }

    if (result == SoundWrapper::Result::OK) {
        ALOGV("Play left turn indicator ok");
    } else if (result == SoundWrapper::Result::PLAY_FAILED) {
        ALOGW("Play left turn indicator fail");
    }

    previous_IndcrDisp1WdSts = to;
}
}
