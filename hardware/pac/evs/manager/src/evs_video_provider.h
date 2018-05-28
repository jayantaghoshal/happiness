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
    explicit EvsVideoProvider(sp<IEvsCamera>&& hw_camera);

    // Factory methods for VirtualCamera:s

    /// Creates a new virtual camera with this EvsVideoProvider as input stream.
    ///
    /// @returns a virtual camera with this EvsVideoProvider as input stream.
    sp<IVirtualCamera> MakeVirtualCamera() override;

    /// Performs a controlled destruction of the input virtual_camera
    ///
    /// Ensures the virtual camera stream is stopped and shut down.
    /// Removes the virtual camera from the list of clients and releases resources
    /// that are no longer needed by it. Sets the input strong pointer to nullptr,
    /// thus destructing the virtual camera.
    ///
    /// @param[in] virtual_camera the virtual camera to destruct.
    void DisownVirtualCamera(sp<IVirtualCamera>& virtual_camera) override;

    // Inline implementations

    /// @returns the hardware camera the class wraps.
    sp<IEvsCamera> GetHwCamera() override { return hw_camera_; };

    /// @returns the number of clients
    std::list<wp<IVirtualCamera>>::size_type GetClientCount() override { return clients_.size(); };

    // Stream handling methods

    /// Checks if hardware camera stream is open, and if not, opens the hardware camera stream.
    ///
    /// Called by client to inform the provider that a stream is needed.
    ///
    /// @returns EvsResult::OK if the stream is STOPPING or RUNNING, else returns the result of the
    /// call to the hardware camera.
    Return<EvsResult> RequestVideoStream() override;

    /// Checks if hardware camera stream is in use, and if not, closes the hardware camera stream.
    ///
    /// Called by client to inform the provider that a stream is no longer needed.
    void ReleaseVideoStream() override;

    /// Return a held frame to the provider
    ///
    /// Called by client to inform the provider that a frame is no longer needed.
    /// Given that the input frame is held, the frames ref count will be reduced by one.
    /// If this brings the ref count to zero, indicating that the frame is no longer in use,
    /// the frame will be returned to the hardware camera. Will not return nullptr frames
    /// since they are not held and should not be returned.
    ///
    /// @param[in] buffer the frame to return.
    void DoneWithFrame(const BufferDesc& buffer) override;

    /// Set the number of frames in flight
    ///
    /// The function will attempt to adjust the number of frames in flight in the hardware camera
    /// to the number needed by the current clients plus the input extra_frames.
    ///
    /// @returns true if the change was successful.
    bool ChangeFramesInFlight(uint32_t extra_frames) override;

    // Methods from ::android::hardware::automotive::evs::V1_0::IEvsCameraStream follow.
    /// @see https://source.android.com/reference/hidl/android/hardware/automotive/evs/1.0/IEvsCameraStream
    ///@{
    Return<void> deliverFrame(const BufferDesc& buffer) override;
    ///@}

  private:
    struct FrameRecord {
        uint32_t frame_id;
        uint32_t ref_count;
        FrameRecord(uint32_t id) : frame_id(id), ref_count(0){};
    };
    using size_type_frames = std::vector<EvsVideoProvider::FrameRecord>::size_type;

    sp<IEvsCamera> hw_camera_;
    std::list<wp<IVirtualCamera>> clients_;  // Weak pointers -> object destructs if client dies
    StreamState output_stream_state_;
    std::vector<FrameRecord> frames_;

    FRIEND_TEST(EvsVideoProviderTest, RequestVideoStreamWhenOutputStreamStopped);
    FRIEND_TEST(EvsVideoProviderTest, RequestVideoStreamWhenOutputStreamRunning);
    FRIEND_TEST(EvsVideoProviderTest, ReleaseVideoStreamLastClient);
    FRIEND_TEST(EvsVideoProviderTest, ReleaseVideoStreamNotLastClient);
    FRIEND_TEST(EvsVideoProviderTest, deliverFrameNoTakers);
    FRIEND_TEST(EvsVideoProviderTest, deliverFrame);
    FRIEND_TEST(EvsVideoProviderTest, ChangeFramesInFlightDecrease);
    FRIEND_TEST(EvsVideoProviderTest, ChangeFramesInFlight);
    FRIEND_TEST(EvsVideoProviderTest, DoneWithFrameNotLastUser);
    FRIEND_TEST(EvsVideoProviderTest, DoneWithFrameLastUser);
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android