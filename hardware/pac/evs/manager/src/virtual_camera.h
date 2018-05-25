/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <deque>

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include <gtest/gtest_prod.h>
#include "i_evs_video_provider.h"
#include "i_virtual_camera.h"
#include "stream_state.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

// The VirtualCamera acts as a middle-man between the consumer of the camera stream and the
// wrapper of the hardware camera (EvsVideoProvider). Many VirtualCamera:s can share the same
// EvsVideoProvider.
class VirtualCamera final : public IVirtualCamera {
  public:
    // Initial maximum number of frames to hold
    static constexpr uint32_t kDefaultFramesAllowed = 6;

    explicit VirtualCamera(const sp<IEvsVideoProvider>& input_stream);
    ~VirtualCamera();

    // Performs a controlled stop of the video stream
    void ShutDown() override;

    // Inline implementations
    sp<IEvsVideoProvider> GetEvsVideoProvider() override { return input_stream_; };
    bool IsStreaming() override { return output_stream_state_ == StreamState::RUNNING; };
    uint32_t GetAllowedBuffers() { return frames_allowed_; };

    // Proxy to receive frames from the input stream and forward them to the output stream
    bool DeliverFrame(const BufferDesc& buffer) override;

    // Methods from ::android::hardware::automotive::evs::V1_0::IEvsCamera follow.
    Return<void> getCameraInfo(getCameraInfo_cb hidl_cb) override;
    Return<EvsResult> setMaxFramesInFlight(uint32_t buffer_count) override;
    Return<EvsResult> startVideoStream(const sp<IEvsCameraStream>& stream) override;
    Return<void> doneWithFrame(const BufferDesc& buffer) override;
    Return<void> stopVideoStream() override;
    Return<int32_t> getExtendedInfo(uint32_t opaque_identifier) override;
    Return<EvsResult> setExtendedInfo(uint32_t opaque_identifier, int32_t opaque_value) override;

  private:
    sp<IEvsVideoProvider> input_stream_;  // The low level camera interface to the hardware camera
    sp<IEvsCameraStream> output_stream_;  // The output stream to the consumer

    std::deque<BufferDesc> frames_held_;
    uint32_t frames_allowed_;

    StreamState output_stream_state_;

    FRIEND_TEST(VirtualCameraTest, startVideoStreamWhenStreamAlreadyRunning);
    FRIEND_TEST(VirtualCameraTest, stopVideoStream);
    FRIEND_TEST(VirtualCameraTest, DeliverFrameOutputStreamStopped);
    FRIEND_TEST(VirtualCameraTest, DeliverFrameEndOfStream);
    FRIEND_TEST(VirtualCameraTest, DeliverFrameTooManyFramesHeld);
    FRIEND_TEST(VirtualCameraTest, DeliverFrame);
    FRIEND_TEST(VirtualCameraTest, doneWithFrame);
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android