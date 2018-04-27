/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "virtual_camera.h"

#include <cinttypes>  //For PRIu32 in dbg messages

#include <libdbg.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

VirtualCamera::VirtualCamera(sp<IEvsVideoProvider> input_stream) : input_stream_(std::move(input_stream)) {}

VirtualCamera::~VirtualCamera() { Shutdown(); }

void VirtualCamera::Shutdown() {
    dbgD("called");

    // Drop our reference to our camera stream provider
    input_stream_ = nullptr;
}

Return<void> VirtualCamera::getCameraInfo(getCameraInfo_cb hidl_cb) {
    // Straight pass through to hardware layer
    return input_stream_->GetHwCamera()->getCameraInfo(hidl_cb);
}

Return<EvsResult> VirtualCamera::setMaxFramesInFlight(uint32_t buffer_count) {
    dbgD("called with buffer_count %" PRIu32, buffer_count);
    // TODO(ihu) Implement VirtualCamera::setMaxFramesInFlight method
    return EvsResult::OK;
}

Return<EvsResult> VirtualCamera::startVideoStream(const sp<IEvsCameraStream>& /*stream*/) {
    dbgD("called");
    // TODO(ihu) Implement VirtualCamera::startVideoStream method
    return EvsResult::OK;
}

Return<void> VirtualCamera::doneWithFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32, buffer.bufferId);
    // TODO(ihu) Implement VirtualCamera::doneWithFrame method
    return Void();
}

Return<void> VirtualCamera::stopVideoStream() {
    dbgD("called");
    // TODO(ihu) Implement VirtualCamera::stopVideoStream method
    return Void();
}

Return<int32_t> VirtualCamera::getExtendedInfo(uint32_t opaque_identifier) {
    // Straight pass through to hardware device
    return input_stream_->GetHwCamera()->getExtendedInfo(opaque_identifier);
}

Return<EvsResult> VirtualCamera::setExtendedInfo(uint32_t opaque_identifier, int32_t opaque_value) {
    // Straight pass through to hardware device
    // TODO(ihu) Consider restricting access to this entry point.
    return input_stream_->GetHwCamera()->setExtendedInfo(opaque_identifier, opaque_value);
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android