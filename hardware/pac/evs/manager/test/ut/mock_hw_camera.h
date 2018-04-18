/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsCamera.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class MockHwCamera : public IEvsCamera {
  public:
    MOCK_METHOD1(getCameraInfo, Return<void>(getCameraInfo_cb hidl_cb));
    MOCK_METHOD1(setMaxFramesInFlight, Return<EvsResult>(uint32_t buffer_count));
    MOCK_METHOD1(startVideoStream, Return<EvsResult>(const sp<IEvsCameraStream>& stream));
    MOCK_METHOD1(doneWithFrame, Return<void>(const BufferDesc& buffer));
    MOCK_METHOD0(stopVideoStream, Return<void>());
    MOCK_METHOD1(getExtendedInfo, Return<int32_t>(uint32_t opaque_identifier));
    MOCK_METHOD2(setExtendedInfo, Return<EvsResult>(uint32_t opaque_identifier, int32_t opaque_value));
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android