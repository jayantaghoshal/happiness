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

    // TODO(ihu) Improve handling of stream state and frames

    // Drop our reference to our camera stream provider
    input_stream_ = nullptr;
}

bool VirtualCamera::DeliverFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32,
         buffer.memHandle != nullptr ? buffer.bufferId : 0);  // Use 0 if buffer is empty

    if (!IsStreaming()) {
        return false;  // Only accept frames if output stream is running
    }

    if (buffer.memHandle == nullptr) {  // A nullptr buffer indicates end of stream
        // Warn that stream termination is unexpected, the stream should be stopped by a call to stopVideoStream().
        dbgE("Input stream stopped unexpectedly");  // TODO(ihu) Downgrade to warning if we restart stack.

        // TODO(ihu) Consider triggering restart of the stack in this case

        // Stop the video stream
        stopVideoStream();
        return false;  // We do hold the buffer, so return false;
    }

    // TODO(ihu) Add handling of max number of frames held.

    // Log our use of frame and forward it to the output stream
    frames_held_.emplace_back(buffer);
    output_stream_->deliverFrame(buffer);
    return true;
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

    if (buffer.memHandle == nullptr) {
        // doneWithFrame() should not be called for end of stream marker.
        dbgE("called with buffer.memHandle == nullptr. Ignoring call.");
        return Void();
    }

    auto it = frames_held_.begin();
    while (it != frames_held_.end()) {
        if (it->bufferId == buffer.bufferId) {
            break;
        }
        ++it;
    }
    if (it == frames_held_.end()) {
        dbgE("called with unrecognized bufferId %" PRIu32 ". Ignoring call.", buffer.bufferId);
        return Void();
    }
    // Remove frame from list of held frames.
    frames_held_.erase(it);

    // Inform parent that we are done with buffer.
    input_stream_->DoneWithFrame(buffer);

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

    // TODO(ihu) Consider handling of currently held frames in this situation

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