/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "evs_enumerator.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

Return<void> EvsEnumerator::getCameraList(getCameraList_cb /* _hidl_cb */) { return Return<void>(); }

Return<sp<IEvsCamera>> EvsEnumerator::openCamera(const hidl_string& /* cameraId */) { return sp<IEvsCamera>(); }

Return<void> EvsEnumerator::closeCamera(const sp<IEvsCamera>& /* car_camera */) { return Return<void>(); }

Return<sp<IEvsDisplay>> EvsEnumerator::openDisplay() { return sp<IEvsDisplay>(); }

Return<void> EvsEnumerator::closeDisplay(const sp<IEvsDisplay>& /* evs_display */) { return Return<void>(); }

Return<DisplayState> EvsEnumerator::getDisplayState() { return DisplayState(); }

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
