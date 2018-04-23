/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsDisplay.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class EvsDrmDisplay : public IEvsDisplay {
  public:
    // Methods from ::android::hardware::automotive::evs::V1_0::IEvsDisplay
    Return<void> getDisplayInfo(getDisplayInfo_cb hidl_cb) override;
    Return<EvsResult> setDisplayState(DisplayState state) override;
    Return<DisplayState> getDisplayState() override;
    Return<void> getTargetBuffer(getTargetBuffer_cb hidl_cb) override;
    Return<EvsResult> returnTargetBufferForDisplay(const BufferDesc& buffer) override;
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
