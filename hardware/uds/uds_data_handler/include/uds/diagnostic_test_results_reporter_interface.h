/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

namespace uds {

class DiagnosticTestResultsReporterInterface {
  public:
    virtual ~DiagnosticTestResultsReporterInterface() = default;
    virtual void ReportTestPass() = 0;
    virtual void ReportTestFail() = 0;
};

}  // namespace uds
