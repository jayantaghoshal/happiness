/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostics_reporter.h"

#include <IDispatcher.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <cassert>
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
constexpr uint64_t DIAGNOSTICS_COOKIE = 0;

enum IFACE { TCAM = 0, MOST = 1, APIX = 2 };

std::mutex diag_service_mutex;

// TODO (Samuel.Idowu): Use header provided by UDS for constants
constexpr uint16_t APIX_DID = 0xEE34;
constexpr uint8_t APIX_OBD_OFF = 0x0000u;
constexpr uint8_t APIX_OBD_ON = 0x0001u;

}  // namespace

namespace vcc {
namespace netman {

using tarmac::eventloop::IDispatcher;

int DiagnosticsReporter::getDtcID(const std::string& iface) {
    if (iface == ifnames_[TCAM]) {
        return TCAM_ID;
    } else if (iface == ifnames_[MOST]) {
        return MOST_ID;
    } else if (iface == ifnames_[APIX]) {
        return APIX_ID;
    }

    // This should never happen
    assert(false);
    return -1;
}

DiagnosticsReporter::DiagnosticsReporter(const vcc::LocalConfigReaderInterface* lcfg) : lcfg_(lcfg) {
    ifnames_[TCAM] = lcfg_->GetString("eth1.name");
    ifnames_[MOST] = lcfg_->GetString("meth0.name");
    ifnames_[APIX] = lcfg_->GetString("eth0.name");

    eth_gw_address_ = lcfg_->GetString("eth1.ip-address");

    apix_traffic_splitting_ = IptablesConfig().isSplitTrafficSet(ifnames_[APIX], eth_gw_address_);

    IDataCollector::registerForNotifications("default", this);
}

void DiagnosticsReporter::check_link() {
    for (const auto& interface : ifnames_) {
        std::string filepath = "/sys/class/net/" + interface + "/carrier";
        int id = getDtcID(interface);

        std::ifstream file(filepath);
        if (file.is_open()) {
            DiagnosticCheckReport report = {DiagnosticCheckStatus::FAILED, 1};

            char state = file.get();
            if (file.good()) {
                report.status = (state == '0') ? DiagnosticCheckStatus::FAILED : DiagnosticCheckStatus::PASSED;
            } else {
                report.status = DiagnosticCheckStatus::FAILED;
            }

            ALOGV("Interface %s Link %c DTC %i", interface.c_str(), state, id);

            std::lock_guard<std::mutex> guard(diag_service_mutex);

            if (!diag_service_.get()) {
                ALOGE("Diagnostics service not reachable");
                return;
            }

            auto uninteresting_retval = diag_service_->reportDiagnosticTestResult(id, report);
            ALOGE_IF(not uninteresting_retval.isOk(), "DiagnosticsReporter, reportDiagnosticsTestResult failed");

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

void DiagnosticsReporter::serviceDied(uint64_t cookie, const ::android::wp<IBase>& who) {
    (void)who;
    if (DIAGNOSTICS_COOKIE == cookie) {
        ALOGE("Diagnostics service died");
        std::lock_guard<std::mutex> guard(diag_service_mutex);
        diag_service_.clear();
    }
}

Return<void> DiagnosticsReporter::onRegistration(const ::android::hardware::hidl_string& fqName,
                                                 const ::android::hardware::hidl_string& name, bool preexisting) {
    (void)fqName;
    (void)name;
    (void)preexisting;
    ALOGI("Diagnostics service reachable");

    std::lock_guard<std::mutex> guard(diag_service_mutex);
    diag_service_ = IDataCollector::getService();

    if (diag_service_.get()) {
        diag_service_->linkToDeath(this, DIAGNOSTICS_COOKIE);
        diag_service_->registerDataHandler(this, {APIX_DID});
    }

    IDispatcher::GetDefaultDispatcher().Enqueue([this] { check_link(); });
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
    if (!IptablesConfig().configureSplitTraffic(ifnames_[APIX], eth_gw_address_, action)) {
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
