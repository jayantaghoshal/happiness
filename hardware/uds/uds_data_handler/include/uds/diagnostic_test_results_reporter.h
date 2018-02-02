/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "diagnostic_test_results_reporter_interface.h"

#include <chrono>
#include <memory>

namespace uds {

class DiagnosticTestResultsReporterImpl;

class DiagnosticTestResultsReporter : public DiagnosticTestResultsReporterInterface {
  public:
    DiagnosticTestResultsReporter(uint32_t dtc_id);
    DiagnosticTestResultsReporter(uint32_t dtc_id, std::chrono::steady_clock::duration reporting_interval);
    ~DiagnosticTestResultsReporter();

    void ReportTestPass() override;
    void ReportTestFail() override;

    std::unique_ptr<DiagnosticTestResultsReporterImpl> pimpl_;
};

}  // namespace uds
