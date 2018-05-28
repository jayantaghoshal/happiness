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

// This interface is a wrapper to allow for easy mocking of the implementation class.
// See the implementation class for details on use.
class IVirtualCamera : public IEvsCamera {
  public:
    // Performs a controlled stop of the video stream
    virtual void ShutDown() = 0;

    // Inline implementations
    virtual sp<IEvsVideoProvider> GetEvsVideoProvider() = 0;
    virtual bool IsStreaming() = 0;
    virtual uint32_t GetAllowedBuffers() = 0;

    virtual bool DeliverFrame(const BufferDesc& buffer) = 0;
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android