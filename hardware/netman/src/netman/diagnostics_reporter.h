/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <utils/StrongPointer.h>
#include <vcc/localconfig.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataHandler.h>

namespace vcc {
namespace netman {

class DiagnosticsReporter final : public ::android::hidl::manager::V1_0::IServiceNotification,
                                  public virtual ::android::RefBase,
                                  public ::android::hardware::hidl_death_recipient,
                                  public ::vendor::volvocars::hardware::uds::V1_0::IDataHandler {
  public:
    explicit DiagnosticsReporter(const vcc::LocalConfigReaderInterface* lcfg = vcc::LocalConfigDefault());

  private:
    void check_link();

    ::android::sp<::vendor::volvocars::hardware::uds::V1_0::IDataCollector> diag_service_;

    ::android::hardware::Return<void> onRegistration(const ::android::hardware::hidl_string& fqName,
                                                     const ::android::hardware::hidl_string& name,
                                                     bool preexisting) override;

    void serviceDied(uint64_t cookie, const ::android::wp<IBase>& who) override;
    int getDtcID(const std::string& iface);

    ::android::hardware::Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;

    ::android::hardware::Return<::vendor::volvocars::hardware::uds::V1_0::DidWriteStatusCode> writeDidValue(
            uint16_t did, const ::android::hardware::hidl_vec<uint8_t>& data) override;

    bool isApixOnObdParamValid(const ::android::hardware::hidl_vec<uint8_t>& data);

    ::android::hardware::Return<::vendor::volvocars::hardware::uds::V1_0::DidWriteStatusCode> setApixOnObd(
            const ::android::hardware::hidl_vec<uint8_t>& data);

    std::array<std::string, 3> ifnames_;
    const vcc::LocalConfigReaderInterface* const lcfg_;
    std::string eth_gw_address_;
    bool apix_traffic_splitting_ = false;
};

}  // namespace netman
}  // namespace vcc
