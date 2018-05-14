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
#include <utils/Errors.h>
#include <utils/StrongPointer.h>

#include <evs/hardware.h>
#include <evs/manager.h>
#include <libdbg.h>
#include <pacconfig/config.h>
#include "evs_enumerator.h"

// libhidl
using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;

// libutils
using ::android::OK;
using ::android::sp;
using ::android::status_t;

// libpacconfig
using ::pac::config::PacConfig;

// thread
using std::this_thread::sleep_for;

// Generated HIDL EVS interface
using ::android::hardware::automotive::evs::V1_0::IEvsEnumerator;

// VCC enumerator implementation: evs_enumerator.h
using ::android::hardware::automotive::evs::V1_0::vcc_implementation::EvsEnumerator;

namespace {
namespace service_cfg {

// Max number of threads the RPC thread pool shall use
constexpr int kMaxNumberOfThreads = 1;

// Indicates that the calling thread shall join the RPC thread pool. The thread pool will create
// one less thread than specified by kMaxNumberOfThreads as it expects the caller to join the pool.
constexpr bool kCallerWillJoin = true;

// How many milliseconds the service shall wait before retrying to get the EVS hardware service
constexpr uint32_t kServiceRetryInterval = 100;
}  // namespace service_cfg
}  // namespace

int main() {
    dbgD("EVS manager service starting...");

    if (!PacConfig::Evs::IsEvsServicesEnabled()) {
        dbgD("EVS manager service did not start since it is disabled");
        return EXIT_FAILURE;
    }

    dbgD("Attempting to connect to EVS hardware service %s", kEvsHardwareServiceName);
    sp<IEvsEnumerator> hwEnumerator;
    do {
        hwEnumerator = IEvsEnumerator::tryGetService(kEvsHardwareServiceName);
        if (hwEnumerator == nullptr) {
            dbgW("Failed to connect to EVS hardware service %s, will retry in %" PRIu32 "ms",
                 kEvsHardwareServiceName,
                 service_cfg::kServiceRetryInterval);
            sleep_for(std::chrono::milliseconds(service_cfg::kServiceRetryInterval));
        }
    } while (hwEnumerator == nullptr);

    sp<EvsEnumerator> enumerator = new EvsEnumerator();
    if (!enumerator->Init(hwEnumerator)) {
        dbgE("Failed to initialize EVS enumerator");
        return EXIT_FAILURE;
    }

    configureRpcThreadpool(service_cfg::kMaxNumberOfThreads, service_cfg::kCallerWillJoin);

    dbgD("Registering EVS manager service as %s", kManagedServiceName);
    status_t status = enumerator->registerAsService(kManagedServiceName);
    if (status != OK) {
        dbgE("Could not register service as %s (%d)", kManagedServiceName, status);
        return EXIT_FAILURE;
    }

    dbgD("EVS manager service %s is ready", kManagedServiceName);

    // Tell the main thread to join the thread pool and handle incoming RPC calls.
    // We do not expect this call to return unless its interrupted or an error occurs.
    joinRpcThreadpool();

    dbgE("EVS manager service is shutting down");
    return EXIT_FAILURE;
}
