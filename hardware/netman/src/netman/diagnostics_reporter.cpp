#include "diagnostics_reporter.h"

#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>
#include <fstream>
#include <mutex>

// TODO Remove
#include <unistd.h>
#include <thread>

#define LOG_TAG "Netmand"
#include <cutils/log.h>

using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::DagnosticCheckStatus;
using ::android::hardware::Return;
using ::android::hardware::Void;

namespace {

constexpr int NUMBER_OF_INTERFACES = 3;
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

int DiagnosticsReporter::getDtcID(const std::string& iface) {
    if (iface == ifnames_[TCAM]) {
        return TCAM_ID;
    } else if (iface == ifnames_[MOST]) {
        return MOST_ID;
    } else {
        return APIX_ID;
    }
}

DiagnosticsReporter::DiagnosticsReporter(const vcc::LocalConfigReaderInterface* lcfg) : lcfg_(lcfg) {
    ifnames_[TCAM] = lcfg_->GetString("eth0.name");
    ifnames_[MOST] = lcfg_->GetString("eth1.name");
    ifnames_[APIX] = lcfg_->GetString("meth0.name");

    IUdsDataCollector::registerForNotifications("default", this);
}

void DiagnosticsReporter::check_link() {
    // TODO Enqueue in event loop
    while (true) {
        for (int i = 0; i < NUMBER_OF_INTERFACES; ++i) {
            std::string interface = ifnames_[i];
            std::string filepath = "/sys/class/net/" + interface + "/carrier";
            int id = getDtcID(interface);

            std::ifstream file(filepath);
            if (file.is_open()) {
                DiagnosticCheckReport report = {DagnosticCheckStatus::FAILED, 1};

                char state = file.get();
                if (file.good()) {
                    report.status = (state == '0') ? DagnosticCheckStatus::FAILED : DagnosticCheckStatus::PASSED;
                } else {
                    report.status = DagnosticCheckStatus::FAILED;
                }

                ALOGV("Interface %s Link %c DTC %i", interface.c_str(), state, id);

                // TODO Enable when diagnostics service is complete
                // std::lock_guard<std::mutex> guard(diag_service_mutex);
                //
                // if (!diag_service_.get()) {
                //     ALOGE("Diagnostics service not reachable");
                //     return;
                // }
                //
                // diag_service_->reportDiagnosticTestResult(id, report);
            } else {
                // As per discussion this case will only happen when the interface is unavailable
                // (whether due to driver issue, netboy or something else) in which case we have other
                // error handling and should not check for link availabilty
                continue;
            }
        }

        // TODO Remove
        usleep(POLLING_INTERVAL_MS * 10000);
    }
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
    diag_service_ = IUdsDataCollector::getService();
    diag_service_->linkToDeath(this, DIAGNOSTICS_COOKIE);
    // TODO Enqueue correctly
    check_link();
    return Void();
}

}  // namespace netman
}  // namespace vcc
