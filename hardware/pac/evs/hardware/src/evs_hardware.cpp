/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <chrono>
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <thread>

#include <hidl/HidlTransportSupport.h>
#include <utils/StrongPointer.h>

#include <evs/hardware.h>
#include <libdbg.h>
#include <pacconfig/config.h>
#include "evs_enumerator.h"

// libhidl
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using android::hardware::automotive::evs::V1_0::IEvsEnumerator;

// libutils
using android::OK;
using android::status_t;

// libpacconfig
using ::pac::config::PacConfig;

// thread
using std::this_thread::sleep_for;

// VCC implementation
using android::hardware::automotive::evs::V1_0::vcc_implementation::EvsEnumerator;

namespace {
namespace service_cfg {
constexpr int kMaxNumberOfThreads = 1;

// Note that maxThreads must include the caller thread if callerWillJoin is true
constexpr bool kCallerWillJoin = true;

// How many milliseconds the service shall wait before retrying to register its enumerator
constexpr uint32_t kRegisterRetryInterval = 100;
}  // namespace service_cfg
}  // namespace

int main() {
    dbgD("EVS hardware service is starting...");

    if (!PacConfig::Evs::IsEvsServicesEnabled()) {
        dbgD("EVS hardware service did not start since it is disabled!");
        return EXIT_SUCCESS;
    }

    configureRpcThreadpool(service_cfg::kMaxNumberOfThreads, service_cfg::kCallerWillJoin);

    android::sp<IEvsEnumerator> service = new EvsEnumerator();
    status_t status;
    do {
        status = service->registerAsService(kEvsHardwareServiceName);
        if (status != OK) {
            dbgW("Failed to register service %s (%d), will retry in %" PRIu32 "ms",
                 kEvsHardwareServiceName,
                 status,
                 service_cfg::kRegisterRetryInterval);
            sleep_for(std::chrono::milliseconds(service_cfg::kRegisterRetryInterval));
        }
    } while (status != OK);

    dbgD("EVS hardware service started. %s is ready!", kEvsHardwareServiceName);
    joinRpcThreadpool();

    // In normal operation, we don't expect the thread pool to exit
    dbgE("EVS hardware service was interrupted, shutting down!");
    return EXIT_FAILURE;
}
