/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <list>

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include <android/hardware/automotive/evs/1.0/IEvsCameraStream.h>
#include "virtual_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class VirtualCamera;  // From virtual_camera.h

// This interface is a wrapper to allow for easy mocking of the implementation class.
// See the implementation class for details on use.
class IEvsCameraStreamWrapper : public IEvsCameraStream {
  public:
    // Factory methods for VirtualCamera:s
    virtual sp<VirtualCamera> MakeVirtualCamera() = 0;
    virtual void DisownVirtualCamera(const sp<VirtualCamera>& virtual_camera) = 0;

    // Inline implementations
    virtual sp<IEvsCamera> GetHwCamera() = 0;
    virtual std::list<wp<VirtualCamera>>::size_type GetClientCount() = 0;
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android