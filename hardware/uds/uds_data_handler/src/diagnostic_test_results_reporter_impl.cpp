/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostic_test_results_reporter_impl.h"

#define LOG_TAG "uds.testresultsreporter"
#include <android-base/logging.h>

using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::toString;
using ::android::hardware::Return;

using uds::DiagnosticTestResultsReporterImpl;

DiagnosticTestResultsReporterImpl::DiagnosticTestResultsReporterImpl(uint32_t dtc_id)
    : DiagnosticTestResultsReporterImpl(dtc_id, std::chrono::seconds(2)) {}

DiagnosticTestResultsReporterImpl::DiagnosticTestResultsReporterImpl(
        uint32_t dtc_id,
        std::chrono::steady_clock::duration reporting_interval)
    : dtc_id_(dtc_id), reporting_interval_(reporting_interval) {}

void DiagnosticTestResultsReporterImpl::ReportTestPass() { ReportStatus(DiagnosticCheckStatus::PASSED); }

void DiagnosticTestResultsReporterImpl::ReportTestFail() { ReportStatus(DiagnosticCheckStatus::FAILED); }

void DiagnosticTestResultsReporterImpl::SendReportToCollector(DiagnosticCheckStatus status,
                                                              std::chrono::steady_clock::time_point now) {
    DiagnosticCheckReport report{status, status_occurences};
    Return<bool> result = data_collector_->reportDiagnosticTestResult(dtc_id_, report);
    if (!result.isOk()) {
        LOG(ERROR) << "Sending report failed over hwbinder despite valid data_collector_: " << result.description();
        data_collector_ = nullptr;

        last_status_ = DiagnosticCheckStatus::NEVER_REPORTED;
        last_report_timepoint_ = now;

    } else {
        LOG(VERBOSE) << "Send report for id " << dtc_id_ << " with " << status_occurences << " occurences of status "
                     << toString(status);
        last_status_ = status;
    }

    // status might change while disconnected so no point in keeping
    status_occurences = 0;
}

void DiagnosticTestResultsReporterImpl::ReportStatus(
        ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus status) {
    if (data_collector_ == nullptr) {
        data_collector_ = ::vendor::volvocars::hardware::uds::V1_0::IDataCollector::tryGetService();
        if (data_collector_ == nullptr) return;
    }

    // valid data_collector_ available
    status_occurences++;

    const auto now = std::chrono::steady_clock::now();
    const auto time_since_last_sent_report = now - last_report_timepoint_;
    const bool reporting_interval_elapsed = time_since_last_sent_report > reporting_interval_;

    const bool status_changed = status != last_status_;
    if (status_changed) status_occurences = 1;

    const bool status_occurences_at_overflow = status_occurences == 0xFFFFu;

    if (reporting_interval_elapsed or status_changed or status_occurences_at_overflow) {
        SendReportToCollector(status, now);
    } else {
        LOG(VERBOSE) << "For dtc id" << dtc_id_ << " increased status_occurences to " << status_occurences;
    }
}
