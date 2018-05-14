/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "pac_early_app.h"

#include <chrono>
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <thread>

#include <android/hardware/automotive/evs/1.0/IEvsEnumerator.h>
#include <utils/StrongPointer.h>

#include <evs/manager.h>
#include <libdbg.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

using android::hardware::automotive::evs::V1_0::IEvsEnumerator;
using android::sp;

// chrono
using std::chrono::milliseconds;

// thread
using std::this_thread::sleep_for;

namespace {
namespace app_cfg {

// How many milliseconds the app shall wait before retrying to get the EVS manager service
constexpr uint32_t kServiceRetryInterval = 100;

}  // namespace app_cfg
}  // namespace

// Main entry point
bool PacEarlyApp::Start() {
    dbgI("PAC app starting...");

    // TODO(ihu) Add configureRpcThreadpool call to be used for HAL events.

    dbgD("App trying to connect to service [%s]", kManagedServiceName);

    // Get the EVS manager service
    sp<IEvsEnumerator> evs;
    do {
        evs = IEvsEnumerator::tryGetService(kManagedServiceName);
        if (evs == nullptr) {
            dbgW("Failed to connect to EVS manager service %s, will retry in %" PRIu32 "ms",
                 kManagedServiceName,
                 app_cfg::kServiceRetryInterval);
            sleep_for(milliseconds(app_cfg::kServiceRetryInterval));
        }
    } while (evs == nullptr);

    dbgD("EVS getService(%s) returned with interface %s", kManagedServiceName, evs->descriptor);

    // Run forever
    dbgI("Entering running state");
    while (true) {
        // TODO(ihu) Add meaningful activity for loop.
        // Sleep to avoid choking CPU
        sleep_for(milliseconds(1000));
    }

    return true;
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
