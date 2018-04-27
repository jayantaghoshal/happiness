/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostic_test_results_reporter.h"

#include "diagnostic_test_results_reporter_impl.h"

#define LOG_TAG "uds.testresultsreporter"
#include <android-base/logging.h>

using uds::DiagnosticTestResultsReporter;
using uds::DiagnosticTestResultsReporterImpl;

DiagnosticTestResultsReporter::~DiagnosticTestResultsReporter() = default;

DiagnosticTestResultsReporter::DiagnosticTestResultsReporter(uint32_t dtc_id)
    : pimpl_(std::make_unique<DiagnosticTestResultsReporterImpl>(dtc_id, std::chrono::seconds(2))) {}

DiagnosticTestResultsReporter::DiagnosticTestResultsReporter(uint32_t dtc_id,
                                                             std::chrono::steady_clock::duration reporting_interval)
    : pimpl_(std::make_unique<DiagnosticTestResultsReporterImpl>(dtc_id, reporting_interval)) {}

void DiagnosticTestResultsReporter::ReportTestPass() {
    pimpl_->ReportTestPass();
}

void DiagnosticTestResultsReporter::ReportTestFail() {
    pimpl_->ReportTestFail();
}
