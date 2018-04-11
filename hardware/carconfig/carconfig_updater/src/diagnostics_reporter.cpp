/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostics_reporter.h"

#include <IDispatcher.h>
#include <time.h>
#include <uds/diagnostic_test_results_reporter.h>
#include <fstream>
#include <mutex>
#define LOG_TAG "carconfig_updater"
#include <cutils/log.h>

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::vendor::volvocars::hardware::uds::V1_0::DidWriteStatusCode;
using ::vendor::volvocars::hardware::uds::V1_0::IDataCollector;

namespace {

constexpr uint16_t CENTRAL_CONFIGURATION_DID_PARAM_FAULTS = 0xE103;
constexpr uint32_t CENTRAL_CONFIGURATION_SYSTEM_NOT_CONFIGURED = 0xE30055;
constexpr uint32_t CENTRAL_CONFIGURATION_SYSTEM_INVALID_CONFIGURATION = 0x30056;

}  // namespace

namespace vcc {
namespace carconfig {

using tarmac::eventloop::IDispatcher;

DiagnosticsReporter::DiagnosticsReporter() {
    central_config_not_configured =
            std::make_unique<uds::DiagnosticTestResultsReporter>(CENTRAL_CONFIGURATION_SYSTEM_NOT_CONFIGURED);
    central_config_invalid_configuration =
            std::make_unique<uds::DiagnosticTestResultsReporter>(CENTRAL_CONFIGURATION_SYSTEM_INVALID_CONFIGURATION);
    IDataCollector::registerForNotifications("default", this);
    service_registered = false;
}

Return<void> DiagnosticsReporter::onRegistration(const ::android::hardware::hidl_string& fqName,
                                                 const ::android::hardware::hidl_string& name,
                                                 bool preexisting) {
    (void)fqName;
    (void)name;
    (void)preexisting;
    ALOGI("Diagnostics service reachable");

    std::lock_guard<std::mutex> lock(mtx);
    diag_service = IDataCollector::getService();
    if (did_data.size() > 0) {
        diag_service->provideStaticDidValue(CENTRAL_CONFIGURATION_DID_PARAM_FAULTS, did_data);
    }
    service_registered = true;
    return Void();
}

Return<void> DiagnosticsReporter::readDidValue(uint16_t did, readDidValue_cb _hidl_cb) { return Void(); }

Return<DidWriteStatusCode> DiagnosticsReporter::writeDidValue(uint16_t did,
                                                              const ::android::hardware::hidl_vec<uint8_t>& data) {
    return DidWriteStatusCode::NOT_SUPPORTED;
}

void DiagnosticsReporter::SetFaultyParamsDid(std::vector<uint8_t> faultyParams) {
    std::lock_guard<std::mutex> lock(mtx);
    if (service_registered) {
        diag_service->provideStaticDidValue(CENTRAL_CONFIGURATION_DID_PARAM_FAULTS, faultyParams);
    } else {
        did_data = faultyParams;
    }
}

void DiagnosticsReporter::waitForRegistration() {
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(30);
    while (std::chrono::steady_clock::now() < deadline) {
        if (service_registered == true) {
            return;
        }
        usleep(1000);
    }
    ALOGE("Diagnostics service unreachable, not able to set DID/DTC");
    return;
}

}  // namespace carconfig
}  // namespace vcc
