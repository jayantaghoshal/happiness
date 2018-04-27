/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <uds/diagnostic_test_results_reporter_interface.h>
#include <vcc/localconfig.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataHandler.h>
#include <array>

namespace vcc {
namespace netman {

class DiagnosticsReporter final : public virtual ::android::RefBase,
                                  public ::android::hidl::manager::V1_0::IServiceNotification,
                                  public ::vendor::volvocars::hardware::uds::V1_0::IDataHandler {
  public:
    explicit DiagnosticsReporter(const vcc::LocalConfigReaderInterface* lcfg = vcc::LocalConfigDefault());

  private:
    // DTC PART
    enum IFACE { TCAM = 0, MOST = 1, APIX = 2, MAX_IFACE_COUNT };

    void check_link();

    struct IfNameWithReporter {
        std::string name;
        std::unique_ptr<uds::DiagnosticTestResultsReporterInterface> reporter;
    };

    ::android::hardware::Return<void> onRegistration(const ::android::hardware::hidl_string& fqName,
                                                     const ::android::hardware::hidl_string& name,
                                                     bool preexisting) override;
    std::array<IfNameWithReporter, MAX_IFACE_COUNT> interfaces_;

    // DID PART
    ::android::hardware::Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;

    ::android::hardware::Return<::vendor::volvocars::hardware::uds::V1_0::DidWriteStatusCode> writeDidValue(
            uint16_t did,
            const ::android::hardware::hidl_vec<uint8_t>& data) override;

    bool isSplitTrafficSet();

    bool isApixOnObdParamValid(const ::android::hardware::hidl_vec<uint8_t>& data);

    ::android::hardware::Return<::vendor::volvocars::hardware::uds::V1_0::DidWriteStatusCode> setApixOnObd(
            const ::android::hardware::hidl_vec<uint8_t>& data);

    std::string eth_gw_address_;
    bool apix_traffic_splitting_ = false;

    // OTHER
    const vcc::LocalConfigReaderInterface* const lcfg_;
};

}  // namespace netman
}  // namespace vcc
