/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <list>

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include <gtest/gtest_prod.h>
#include "i_evs_video_provider.h"
#include "stream_state.h"
#include "virtual_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

// This class wraps the actual hardware IEvsCamera objects. There is a one to many relationship
// between instances of this class and instances of the VirtualCamera class. This class implements
// the HIDL IEvsCameraStream interface, via the IEvsVideoProvider wrapper interface, so that it
// can receive video from the hardware camera and distribute it to the associated VirtualCamera
// objects.
class EvsVideoProvider : public IEvsVideoProvider {
  public:
    explicit EvsVideoProvider(const sp<IEvsCamera>& hw_camera) : hw_camera_(hw_camera){};

    // Factory methods for VirtualCamera:s
    sp<IVirtualCamera> MakeVirtualCamera() override;
    void DisownVirtualCamera(const sp<IVirtualCamera>& virtual_camera) override;

    // Inline implementations
    sp<IEvsCamera> GetHwCamera() override { return hw_camera_; };
    std::list<wp<IVirtualCamera>>::size_type GetClientCount() override { return clients_.size(); };

    // Stream handling methods
    Return<EvsResult> RequestVideoStream() override;
    void ReleaseVideoStream() override;
    void DoneWithFrame(const BufferDesc& buffer) override;

    // Methods from ::android::hardware::automotive::evs::V1_0::IEvsCameraStream follow.
    Return<void> deliverFrame(const BufferDesc& buffer) override;

  private:
    sp<IEvsCamera> hw_camera_;
    std::list<wp<IVirtualCamera>> clients_;  // Weak pointers -> object destructs if client dies
    StreamState output_stream_state_ = StreamState::STOPPED;

    struct FrameRecord {
        uint32_t frame_id;
        uint32_t ref_count;
        FrameRecord(uint32_t id) : frame_id(id), ref_count(0){};
    };
    std::vector<FrameRecord> frames_;

    FRIEND_TEST(EvsVideoProviderTest, RequestVideoStreamWhenOutputStreamStopped);
    FRIEND_TEST(EvsVideoProviderTest, RequestVideoStreamWhenOutputStreamRunning);
    FRIEND_TEST(EvsVideoProviderTest, ReleaseVideoStreamNotLastClient);
    FRIEND_TEST(EvsVideoProviderTest, deliverFrameNoTakers);
    FRIEND_TEST(EvsVideoProviderTest, deliverFrame);
    FRIEND_TEST(EvsVideoProviderTest, DoneWithFrameNotLastUser);
    FRIEND_TEST(EvsVideoProviderTest, DoneWithFrameLastUser);
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android