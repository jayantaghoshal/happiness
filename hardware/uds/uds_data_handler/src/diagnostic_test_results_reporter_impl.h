/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "diagnostic_test_results_reporter_interface.h"

#include <chrono>

#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>

namespace uds {

class DiagnosticTestResultsReporterImpl {
  public:
    DiagnosticTestResultsReporterImpl(uint32_t dtc_id);
    DiagnosticTestResultsReporterImpl(uint32_t dtc_id, std::chrono::steady_clock::duration reporting_interval);

    void ReportTestPass();
    void ReportTestFail();

  private:
    void ReportStatus(::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus status);
    void SendReportToCollector(::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus status,
                               std::chrono::steady_clock::time_point now);

    const uint32_t dtc_id_;
    const std::chrono::steady_clock::duration reporting_interval_;

    ::android::sp<::vendor::volvocars::hardware::uds::V1_0::IDataCollector> data_collector_ = nullptr;

    ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus last_status_ =
            ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus::NEVER_REPORTED;
    uint16_t status_occurences = 0;
    std::chrono::steady_clock::time_point last_report_timepoint_ = std::chrono::steady_clock::time_point();
};

}  // namespace uds
