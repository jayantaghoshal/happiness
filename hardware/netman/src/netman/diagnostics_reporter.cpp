#include "diagnostics_reporter.h"

#include <IDispatcher.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <cassert>
#include <fstream>
#include <mutex>

#define LOG_TAG "Netmand"
#include <cutils/log.h>

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::IDataCollector;

namespace {

constexpr int TCAM_ID = 0x960900;
constexpr int MOST_ID = 0x960A00;
constexpr int APIX_ID = 0x960B00;
constexpr int POLLING_INTERVAL_MS = 100;
constexpr uint64_t DIAGNOSTICS_COOKIE = 0;

enum IFACE { TCAM = 0, MOST = 1, APIX = 2 };

std::mutex diag_service_mutex;

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
    ifnames_[TCAM] = lcfg_->GetString("eth0.name");
    ifnames_[MOST] = lcfg_->GetString("eth1.name");
    ifnames_[APIX] = lcfg_->GetString("meth0.name");

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
    }

    IDispatcher::GetDefaultDispatcher().Enqueue([this] { check_link(); });
    return Void();
}

}  // namespace netman
}  // namespace vcc
