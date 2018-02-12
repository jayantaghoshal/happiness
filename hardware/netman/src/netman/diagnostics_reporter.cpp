/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostics_reporter.h"

#include <IDispatcher.h>
#include <uds/diagnostic_test_results_reporter.h>
#include <fstream>
#include <mutex>
#include "iptables_config.h"

#define LOG_TAG "Netmand"
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

constexpr int TCAM_ID = 0x960900;
constexpr int MOST_ID = 0x960A00;
constexpr int APIX_ID = 0x960B00;

constexpr int POLLING_INTERVAL_MS = 100;

// TODO (Samuel.Idowu): Use header provided by UDS for constants
constexpr uint16_t APIX_DID = 0xEE34;
constexpr uint8_t APIX_OBD_OFF = 0x00u;
constexpr uint8_t APIX_OBD_ON = 0x01u;

}  // namespace

namespace vcc {
namespace netman {

using tarmac::eventloop::IDispatcher;

DiagnosticsReporter::DiagnosticsReporter(const vcc::LocalConfigReaderInterface* lcfg) : lcfg_(lcfg) {
    interfaces_[APIX] = {"apix0", std::make_unique<uds::DiagnosticTestResultsReporter>(APIX_ID)};
    interfaces_[TCAM] = {"tcam0", std::make_unique<uds::DiagnosticTestResultsReporter>(TCAM_ID)};
    interfaces_[MOST] = {"meth0", std::make_unique<uds::DiagnosticTestResultsReporter>(MOST_ID)};

    eth_gw_address_ = lcfg_->GetString("tcam0", "ip-address");

    apix_traffic_splitting_ = IptablesConfig().isSplitTrafficSet(interfaces_[APIX].name, eth_gw_address_);

    IDataCollector::registerForNotifications("default", this);
    IDispatcher::GetDefaultDispatcher().Enqueue([this] { check_link(); });
}

void DiagnosticsReporter::check_link() {
    for (const auto& interface : interfaces_) {
        std::string filepath = "/sys/class/net/" + interface.name + "/carrier";

        std::ifstream file(filepath);
        if (file.is_open()) {
            char state = file.get();
            if (file.good() and state != '0') {
                interface.reporter->ReportTestPass();
            } else {
                interface.reporter->ReportTestFail();
            }

            ALOGV("Interface %s Link %c", interface.name.c_str(), state);
        } else {
            // As per discussion this case will only happen when the interface is unavailable
            // (whether due to driver issue, netboy or something else) in which case we have other
            // error handling and should not check for link availabilty
            continue;
        }
    }

    IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(std::chrono::milliseconds(POLLING_INTERVAL_MS),
                                                         [this] { check_link(); });
}

Return<void> DiagnosticsReporter::onRegistration(const ::android::hardware::hidl_string& fqName,
                                                 const ::android::hardware::hidl_string& name, bool preexisting) {
    (void)fqName;
    (void)name;
    (void)preexisting;
    ALOGI("Diagnostics service reachable");

    auto diag_service = IDataCollector::getService();

    if (diag_service != nullptr) {
        diag_service->registerDataHandler(this, {APIX_DID});
    }

    return Void();
}

Return<void> DiagnosticsReporter::readDidValue(uint16_t did, readDidValue_cb _hidl_cb) {
    switch (did) {
        case APIX_DID: {
            uint8_t resp = apix_traffic_splitting_ ? APIX_OBD_ON : APIX_OBD_OFF;
            _hidl_cb(DidReadResult{DidReadStatusCode::SUCCESS, {resp}});
            break;
        }
        default: {
            ALOGE("ReadDid: Unknown DID received");
            _hidl_cb(DidReadResult{DidReadStatusCode::NOT_SUPPORTED, {}});
            break;
        }
    }

    return Void();
}

Return<DidWriteStatusCode> DiagnosticsReporter::writeDidValue(uint16_t did,
                                                              const ::android::hardware::hidl_vec<uint8_t>& data) {
    switch (did) {
        case APIX_DID:
            return setApixOnObd(data);
        default:
            ALOGE("Invalid DID received");
            return DidWriteStatusCode::NOT_SUPPORTED;
    }
}

Return<DidWriteStatusCode> DiagnosticsReporter::setApixOnObd(const ::android::hardware::hidl_vec<uint8_t>& data) {
    if (!isApixOnObdParamValid(data)) {
        ALOGE("Invalid DID value received");
        return DidWriteStatusCode::NOT_SUPPORTED;
    }

    bool action = (APIX_OBD_ON == data[0]);

    // Configure iptables to mirror APIX traffic to TCAM
    if (!IptablesConfig().configureSplitTraffic(interfaces_[APIX].name, eth_gw_address_, action)) {
        ALOGE("Failed to configure APIX-OBD traffic split");
        return DidWriteStatusCode::CONDITIONS_NOT_CORRECT;
    }

    apix_traffic_splitting_ = action;
    return DidWriteStatusCode::SUCCESS;
}

bool DiagnosticsReporter::isApixOnObdParamValid(const ::android::hardware::hidl_vec<uint8_t>& data) {
    if (data[0] == APIX_OBD_ON || data[0] == APIX_OBD_OFF) {
        return true;
    }
    return false;
}

}  // namespace netman
}  // namespace vcc
