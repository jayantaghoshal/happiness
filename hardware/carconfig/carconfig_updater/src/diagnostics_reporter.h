/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <uds/diagnostic_test_results_reporter_interface.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataHandler.h>
#include <array>

namespace vcc {
namespace carconfig {

using ::android::sp;

class DiagnosticsReporter final : public virtual ::android::RefBase,
                                  public ::android::hidl::manager::V1_0::IServiceNotification,
                                  public ::vendor::volvocars::hardware::uds::V1_0::IDataHandler {
  public:
    explicit DiagnosticsReporter();
    void SetFaultyParamsDid(std::vector<uint8_t> faultyParams);
    void waitForRegistration();
    std::unique_ptr<uds::DiagnosticTestResultsReporterInterface> central_config_not_configured;
    std::unique_ptr<uds::DiagnosticTestResultsReporterInterface> central_config_invalid_configuration;

  private:
    ::android::hardware::Return<void> onRegistration(const ::android::hardware::hidl_string& fqName,
                                                     const ::android::hardware::hidl_string& name,
                                                     bool preexisting) override;

    ::android::hardware::Return<::vendor::volvocars::hardware::uds::V1_0::DidWriteStatusCode> writeDidValue(
            uint16_t did,
            const ::android::hardware::hidl_vec<uint8_t>& data) override;

    ::android::hardware::Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;

    sp<::vendor::volvocars::hardware::uds::V1_0::IDataCollector> diag_service;
    std::vector<uint8_t> did_data;
    std::atomic_bool service_registered;
    std::mutex mtx;
};

}  // namespace carconfig
}  // namespace vcc
