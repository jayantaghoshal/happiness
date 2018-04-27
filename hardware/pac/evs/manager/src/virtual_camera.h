/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include "i_evs_video_provider.h"
#include "i_virtual_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

// The VirtualCamera acts as a middle-man between the consumer of the camera stream and the
// wrapper of the hardware camera (EvsCameraStream). Many VirtualCamera:s can share the same
// EvsCameraStream.
class VirtualCamera final : public IVirtualCamera {
  public:
    explicit VirtualCamera(sp<IEvsVideoProvider> input_stream);
    ~VirtualCamera();

    // Performs a controlled stop of the video stream
    void Shutdown() override;

    // Inline implementations
    sp<IEvsVideoProvider> GetEvsVideoProvider() override { return input_stream_; };

    // Methods from ::android::hardware::automotive::evs::V1_0::IEvsCamera follow.
    Return<void> getCameraInfo(getCameraInfo_cb hidl_cb) override;
    Return<EvsResult> setMaxFramesInFlight(uint32_t buffer_count) override;
    Return<EvsResult> startVideoStream(const sp<IEvsCameraStream>& stream) override;
    Return<void> doneWithFrame(const BufferDesc& buffer) override;
    Return<void> stopVideoStream() override;
    Return<int32_t> getExtendedInfo(uint32_t opaque_identifier) override;
    Return<EvsResult> setExtendedInfo(uint32_t opaque_identifier, int32_t opaque_value) override;

  private:
    sp<IEvsVideoProvider> input_stream_;  // The low level camera interface
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android