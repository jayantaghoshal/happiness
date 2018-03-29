/*
 * Copyright 2017-2018 Volvo Car Corporation
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

namespace SoundNotifications {
TurnIndicator::TurnIndicator() : previous_IndcrDisp1WdSts(IndcrSts1::Off) {
    ALOGI("TurnIndicator::TurnIndicator()");
    indcrRecevier.subscribe([&]() {
        ALOGD("indcrRecevier received - trigger to play indicator sound");

        injectSignals(indcrRecevier.get(),
                      fltIndLeFrontReceiver.get(),
                      fltIndLeReReceiver.get(),
                      fltIndRiFrontReceiver.get(),
                      fltIndRiReReceiver.get());

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

    if (to == IndcrSts1::LeOn) {
        if (from == IndcrSts1::Off || from == IndcrSts1::RiOn || from == IndcrSts1::LeAndRiOn) {
            if (FltIndcrTurnLeFrnt.isError() || FltIndcrTurnLeRe.isError()) {
                ALOGW("No turnindicator left sound played because FltIndcr signal not valid");
            } else if ((FltIndcrTurnLeFrnt.value() == DevErrSts2::NoFlt) &&
                       (FltIndcrTurnLeRe.value() == DevErrSts2::NoFlt)) {
                SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                         AudioTable::SoundComponent::LeftRight));
            } else {
                SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                         AudioTable::SoundComponent::LeftRightBroken));
            }
        }
    } else if (to == IndcrSts1::RiOn) {
        if (from == IndcrSts1::Off || from == IndcrSts1::LeOn || from == IndcrSts1::LeAndRiOn) {
            if (FltIndcrTurnRiFrnt.isError() || FltIndcrTurnRiRe.isError()) {
                ALOGW("No turnindicator right sound played because FltIndcr signal not valid");
            } else if ((FltIndcrTurnRiFrnt.value() == DevErrSts2::NoFlt) &&
                       (FltIndcrTurnRiRe.value() == DevErrSts2::NoFlt)) {
                SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                         AudioTable::SoundComponent::LeftRight));

            } else {
                SoundWrapper::play(SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator,
                                                         AudioTable::SoundComponent::LeftRightBroken));
            }
        }
    } else if (to == IndcrSts1::LeAndRiOn) {
        if (from == IndcrSts1::Off || from == IndcrSts1::LeOn || from == IndcrSts1::RiOn) {
            SoundWrapper::play(
                    SoundWrapper::SoundID(AudioTable::SoundType::TurnIndicator, AudioTable::SoundComponent::Hazard));
        }
    } else {
        // off
    }

    previous_IndcrDisp1WdSts = to;
}
}
