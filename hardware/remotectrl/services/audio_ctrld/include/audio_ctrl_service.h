/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <mutex>
#include <vsomeip/vsomeip.hpp>

#include <vendor/volvocars/hardware/remotectrl/1.0/INativeRemoteAudioCtrl.h>
#include <vendor/volvocars/hardware/remotectrl/1.0/types.h>

#include "service_base.h"

namespace vcc {
namespace remotectrl {
namespace remoteaudioctrl {

using ::android::hardware::Return;
using ::android::hardware::Void;

namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

class AudioCtrlService final : public hidl_remotectrl::INativeRemoteAudioCtrl,
                               public vcc::remotectrl::servicebase::ServiceBase,
                               ::android::hardware::hidl_death_recipient {
  public:
    explicit AudioCtrlService(const ServiceInfo& service_info) : ServiceBase(service_info) {}

    Return<void> registerAudioControlHandler(
            const ::android::sp<hidl_remotectrl::ISystemRemoteAudioCtrl>& handler) override;

    Return<void> unregisterAudioControlHandler(
            const ::android::sp<hidl_remotectrl::ISystemRemoteAudioCtrl>& handler) override;

    Return<void> updateVolume(const hidl_remotectrl::VolumeData& data) override;

    Return<void> sendGetVolumeResp(uint16_t requestIdentifier,
                                   hidl_remotectrl::StatusCode statusCode,
                                   const hidl_remotectrl::VolumeData& data) override;

    Return<void> sendSetVolumeResp(uint16_t requestIdentifier, hidl_remotectrl::StatusCode statusCode) override;

  private:
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

    void OnStateChange(vsomeip::state_type_e state);

    void OnMessageReceive(const std::shared_ptr<vsomeip::message>& message);

    std::mutex guard_;
    ::android::sp<hidl_remotectrl::ISystemRemoteAudioCtrl> system_service_handler_;
};

}  // namespace remoteaudioctrl
}  // namespace remotectrl
}  // namespace vcc
