/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <list>

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include "i_evs_camera_stream_wrapper.h"
#include "virtual_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

// This class wraps the actual hardware IEvsCamera objects. There is a one to many relationship
// between instances of this class and instances of the VirtualCamera class. This class implements
// the IEvsCameraStream interface, via the IEvsCameraStreamWrapper wrapper interface, so that it
// can receive the video stream from the hardware camera and distribute it to the associated
// VirtualCamera objects.
class EvsCameraStream : public IEvsCameraStreamWrapper {
  public:
    explicit EvsCameraStream(const sp<IEvsCamera>& hw_camera) : hw_camera_(hw_camera){};

    // Factory methods for VirtualCamera:s
    sp<IVirtualCamera> MakeVirtualCamera() override;
    void DisownVirtualCamera(const sp<IVirtualCamera>& virtual_camera) override;

    // Inline implementations
    sp<IEvsCamera> GetHwCamera() override { return hw_camera_; };
    std::list<wp<IVirtualCamera>>::size_type GetClientCount() override { return clients_.size(); };

    // Methods from ::android::hardware::automotive::evs::V1_0::IEvsCameraStream follow.
    Return<void> deliverFrame(const BufferDesc& buffer) override;

  private:
    sp<IEvsCamera> hw_camera_;
    std::list<wp<IVirtualCamera>> clients_;  // Weak pointers -> object destructs if client dies
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android