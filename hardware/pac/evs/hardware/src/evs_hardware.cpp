/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdlib>

#include <cutils/properties.h>
#include <hidl/HidlTransportSupport.h>
#include <utils/StrongPointer.h>

#include <libdbg.h>
#include <pacconfig/config.h>
#include "evs/hardware.h"
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

// VCC implementation
using android::hardware::automotive::evs::V1_0::vcc_implementation::EvsEnumerator;

namespace {
namespace service_cfg {
constexpr int kMaxNumberOfThreads = 1;
// Note that maxThreads must include the caller thread if callerWillJoin is true
constexpr bool kCallerWillJoin = true;
}  // namespace service_cfg
}  // namespace

int main() {
    dbgD("EVS hardware service is starting...");
    android::sp<IEvsEnumerator> service;
    status_t status;

    if (!PacConfig::Evs::IsEvsServicesEnabled()) {
        dbgD("EVS hardware service did not start since it is disabled!");
        return EXIT_SUCCESS;
    }

    service = new EvsEnumerator();
    configureRpcThreadpool(service_cfg::kMaxNumberOfThreads, service_cfg::kCallerWillJoin);
    status = service->registerAsService(kEvsHardwareServiceName);
    if (status == OK) {
        dbgD("EVS hardware service started. %s is ready!", kEvsHardwareServiceName);

        joinRpcThreadpool();

        // In normal operation, we don't expect the thread pool to exit
        dbgE("EVS hardware service was interrupted, shutting down!");
    } else {
        dbgE("Could not register service %s (%d).", kEvsHardwareServiceName, status);
    }

    return EXIT_FAILURE;
}
