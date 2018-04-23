/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "evs_enumerator.h"

#include <libdbg.h>
#include "evs_drm_display.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

Return<void> EvsEnumerator::getCameraList(getCameraList_cb /* hidl_cb */) { return Return<void>(); }

Return<sp<IEvsCamera>> EvsEnumerator::openCamera(const hidl_string& /* camera_id */) { return sp<IEvsCamera>(); }

Return<void> EvsEnumerator::closeCamera(const sp<IEvsCamera>& /* car_camera */) { return Return<void>(); }

Return<sp<IEvsDisplay>> EvsEnumerator::openDisplay() {
    // Only one caller may have access to the display at any given time. Therefore, if there is already
    // an active display it needs to be closed before exclusive access can be given to the new caller.
    sp<IEvsDisplay> current_active_display = active_display_.promote();
    if (current_active_display != nullptr) {
        dbgW("Killing active display due to new caller");
        closeDisplay(current_active_display);
    }

    current_active_display = new EvsDrmDisplay();
    active_display_ = current_active_display;

    return current_active_display;
}

Return<void> EvsEnumerator::closeDisplay(const sp<IEvsDisplay>& display) {
    sp<IEvsDisplay> current_active_display = active_display_.promote();

    if (current_active_display == nullptr) {
        dbgE("Requested to close display when there isn't one active");
    } else if (current_active_display != display) {
        dbgW("Ignoring request to close orphaned display");
    } else {
        active_display_ = nullptr;
    }

    return Void();
}

Return<DisplayState> EvsEnumerator::getDisplayState() {
    sp<IEvsDisplay> current_active_display = active_display_.promote();

    if (current_active_display == nullptr) {
        return DisplayState::NOT_OPEN;
    }

    return current_active_display->getDisplayState();
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
