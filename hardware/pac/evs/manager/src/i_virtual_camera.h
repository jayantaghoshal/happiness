/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>
#include "i_evs_video_provider.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class IEvsVideoProvider;  // From i_evs_video_provider.h

// The VirtualCamera acts as a middle-man between the consumer of the camera stream and the wrapper of the hardware
// camera (EvsCameraStream). Many VirtualCamera:s can share the same EvsCameraStream.
class IVirtualCamera : public IEvsCamera {
  public:
    // Performs a controlled stop of the video stream
    virtual void Shutdown() = 0;

    // Inline implementations
    virtual sp<IEvsVideoProvider> GetEvsVideoProvider() = 0;
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android