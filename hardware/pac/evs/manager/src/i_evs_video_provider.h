/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <list>

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include <android/hardware/automotive/evs/1.0/IEvsCameraStream.h>
#include "i_virtual_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

// This interface is a wrapper to allow for easy mocking of the implementation class.
// See the implementation class for details on use.
class IEvsVideoProvider : public IEvsCameraStream {
  public:
    // Factory methods for VirtualCamera:s
    virtual sp<IVirtualCamera> MakeVirtualCamera() = 0;
    virtual void DisownVirtualCamera(sp<IVirtualCamera>& virtual_camera) = 0;

    // Inline implementations
    virtual sp<IEvsCamera> GetHwCamera() = 0;
    virtual std::list<wp<IVirtualCamera>>::size_type GetClientCount() = 0;

    // Stream handling methods
    virtual Return<EvsResult> RequestVideoStream() = 0;
    virtual void ReleaseVideoStream() = 0;
    virtual void DoneWithFrame(const BufferDesc& buffer) = 0;
    virtual bool ChangeFramesInFlight(uint32_t extra_frames) = 0;
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android