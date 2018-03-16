/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsEnumerator.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

// Implements the public interface that will handle RPC calls from applications that
// want to get access to EVS camera or display hardware.
//
// Before this class can be registered as a service and handle RPC calls it needs to
// be initialized by calling the Init() method.
class EvsEnumerator : public IEvsEnumerator {
  public:
    // Initializes the enumerator with the specified hardware enumerator. Calling this
    // method again after initialization has succeeded will always return false.
    bool Init(const sp<IEvsEnumerator>& hw_enumerator);

    // Methods from ::android::hardware::automotive::evs::V1_0:IEvsEnumerator
    Return<void> getCameraList(getCameraList_cb list_cb) override;
    Return<sp<IEvsCamera>> openCamera(const hidl_string& camera_id) override;
    Return<void> closeCamera(const sp<IEvsCamera>& car_camera) override;
    Return<sp<IEvsDisplay>> openDisplay() override;
    Return<void> closeDisplay(const sp<IEvsDisplay>& display) override;
    Return<DisplayState> getDisplayState() override;

  private:
    // EVS hardware service enumerator
    sp<IEvsEnumerator> hw_enumerator_;
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
