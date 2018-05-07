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

Return<EvsResult> VirtualCamera::startVideoStream(const sp<IEvsCameraStream>& stream) {
    dbgD("called");
    // Each VirtualCamera supports one stream at a time.
    if (output_stream_state_ != StreamState::STOPPED) {
        dbgE("ignoring startVideoStream call since a stream is already running.");
        return EvsResult::STREAM_ALREADY_RUNNING;
    }

    // TODO(ihu) Add frame handling

    // Inform the input stream provider that we want to stream
    Return<EvsResult> result = input_stream_->RequestVideoStream();
    if (!result.isOk() || result != EvsResult::OK) {
        return EvsResult::UNDERLYING_SERVICE_ERROR;
    }

    // Set output stream and its state
    output_stream_ = stream;
    output_stream_state_ = StreamState::RUNNING;

    // TODO(ihu) Consider adding handling of e.g. a stalled stream or unresponsive driver
    return EvsResult::OK;
}

Return<void> VirtualCamera::doneWithFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32,
         buffer.memHandle != nullptr ? buffer.bufferId : 0);  // Use 0 if buffer is empty
    // TODO(ihu) Implement VirtualCamera::doneWithFrame method
    return Void();
}

Return<void> VirtualCamera::stopVideoStream() {
    dbgD("called");

    // If we are not streaming, return.
    if (!IsStreaming()) {
        return Void();
    }

    // Inform the input stream that we don't want any more frames
    output_stream_state_ = StreamState::STOPPING;

    // Deliver an empty frame to close the output stream
    BufferDesc empty_frame = {};
    auto result = output_stream_->deliverFrame(empty_frame);
    if (!result.isOk()) {
        dbgE("Error delivering end of stream marker");
    }

    // Since we are single threaded, no frame can be delivered while this function is running,
    // so we can go directly to the STOPPED state here on the server.
    // Note, however, that there still might be frames already queued that client will see
    // after returning from the client side of this call.
    output_stream_state_ = StreamState::STOPPED;

    // Inform the input stream that we no longer need it.
    input_stream_->ReleaseVideoStream();

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