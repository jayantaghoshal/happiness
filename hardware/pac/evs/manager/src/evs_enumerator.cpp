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

bool EvsEnumerator::Init(const sp<IEvsEnumerator>& hw_enumerator) {
    // Check if the enumerator has already been initialized
    if (hw_enumerator_ != nullptr) {
        return false;
    }

    hw_enumerator_ = hw_enumerator;
    return (hw_enumerator_ != nullptr);
}

Return<void> EvsEnumerator::getCameraList(getCameraList_cb /*list_cb*/) {
    return Void();
}

Return<sp<IEvsCamera>> EvsEnumerator::openCamera(const hidl_string& /*camera_id*/) {
    return sp<IEvsCamera>();
}

Return<void> EvsEnumerator::closeCamera(const sp<IEvsCamera>& /*car_camera*/) {
    return Void();
}

Return<sp<IEvsDisplay>> EvsEnumerator::openDisplay() {
    return sp<IEvsDisplay>();
}

Return<void> EvsEnumerator::closeDisplay(const sp<IEvsDisplay>& /*display*/) {
    return Void();
}

Return<DisplayState> EvsEnumerator::getDisplayState() {
    return DisplayState();
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
