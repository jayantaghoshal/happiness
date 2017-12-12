/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vfc_handler.h"
#include <chrono>

namespace {
// The amount of time during which no new VFCs are sent
const std::chrono::milliseconds passiveInterval(500);

// Time when the last VFC for UserInputSettings was sent
// (To support more VFCs then just create an array of these timestamps)
std::chrono::steady_clock::time_point userInputSettings_timestamp;
uint8_t userInputSettings_activeTime(0);
}

/*
 * To limit the number of activatevfc messages sent to the VIP over uart1 we
 * here
 * handle a rate limiter.
 *
 * Currently we only handle the UserInputSettings since that is the one
 * imposing problems.
 */
bool shallSendVfcToVIP(const ActivateVfc &vfc) {
    if (vfc.vfcToActivate == Vfc::UserInputSettings) {
        const auto now = std::chrono::steady_clock::now();
        const auto timeSinceLastActivation = now - userInputSettings_timestamp;
        if ((timeSinceLastActivation > passiveInterval) || (vfc.secondsToKeepActive > userInputSettings_activeTime)) {
            // this vfc was activated more than 500ms ago or
            // the new activation time is longer so lets issue a new request
            userInputSettings_timestamp = now;
            userInputSettings_activeTime = vfc.secondsToKeepActive;
            return true;
        } else {
            return false;  // Dont send since we did it very recently
        }
    } else {
        return true;  // Always send for these VFCs
    }
}
