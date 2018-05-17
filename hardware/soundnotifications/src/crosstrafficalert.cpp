/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <cutils/log.h>

#include "crosstrafficalert.h"

using namespace ApplicationDataElement;
using namespace autosar;

#undef LOG_TAG
#define LOG_TAG "SoundNotifyCTA"

namespace SoundNotifications {
CTAWarning::CTAWarning() {
    ALOGI("CTAWarning::CTAWarning()");

    auto handle_signals = [this] {
        const auto signal = veh_mod_mngt_glb_safe1.get();
        const bool _cta_active = (signal.isOk() && signal.value().UsgModSts == UsgModSts1::UsgModDrvg &&
                                  signal.value().CarModSts1_ == CarModSts1::CarModNorm);

        // if cta activation changed, stop or start the sounds
        if (cta_active != _cta_active) {
            cta_active = _cta_active;
            ALOGD("CTAWarning:: vehicle mode changed %i", cta_active);
            InjectSignalsRight();
            InjectSignalsLeft();
        }
    };

    // Removed checking CarConfig parameter 152 value 0x02 due to change requirements in
    // https://flow.jira.cm.volvocars.biz/browse/ARTINFO-2170

    ctra_indcn_le.subscribe([this] { InjectSignalsLeft(); });
    ctra_indcn_ri.subscribe([this] { InjectSignalsRight(); });

    // subscribe to Vehicle management status
    veh_mod_mngt_glb_safe1.subscribe(handle_signals);
}

void CTAWarning::InjectSignalsRight() {
    const auto signal = ctra_indcn_ri.get();

    // play sound until one of the signals (or both) equals CtraWarn
    const bool _ind_play = (signal.isOk() && (signal.value() == autosar::CtraIndcn1::CtraWarn));

    if (_ind_play && cta_active && !play_sound_right.isValid) {
        // play only if car status is correct: UsgModDrvg && CarModNorm
        play_sound_right.isValid = true;
        SoundWrapper::play(play_sound_right);
    }
    if ((!_ind_play || !cta_active) && play_sound_right.isValid) {
        SoundWrapper::stop(play_sound_right);
        play_sound_right.isValid = false;
    }
}

void CTAWarning::InjectSignalsLeft() {
    /* REQPROD:243856/MAIN;3	Audio Request for Cross Traffic Alert */

    const auto signal = ctra_indcn_le.get();

    // play sound until one of the signals (or both) equals CtraWarn
    const bool _ind_play = (signal.isOk() && (signal.value() == autosar::CtraIndcn1::CtraWarn));

    if (_ind_play && cta_active && !play_sound_left.isValid) {
        // play only if car status is correct: UsgModDrvg && CarModNorm
        play_sound_left.isValid = true;
        SoundWrapper::play(play_sound_left);
    }

    if ((!_ind_play || !cta_active) && play_sound_left.isValid) {
        SoundWrapper::stop(play_sound_left);
        play_sound_left.isValid = false;
    }
}
}  // namespace SoundNotifications
