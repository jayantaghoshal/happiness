/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "evs_drm_display.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

Return<void> EvsDrmDisplay::getDisplayInfo(getDisplayInfo_cb /* hidl_cb */) { return Return<void>(); }

Return<EvsResult> EvsDrmDisplay::setDisplayState(DisplayState /* state */) { return EvsResult::OK; }

Return<DisplayState> EvsDrmDisplay::getDisplayState() { return DisplayState::DEAD; }

Return<void> EvsDrmDisplay::getTargetBuffer(getTargetBuffer_cb /* hidl_cb */) { return Return<void>(); }

Return<EvsResult> EvsDrmDisplay::returnTargetBufferForDisplay(const BufferDesc& /* buffer */) { return EvsResult::OK; }

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
