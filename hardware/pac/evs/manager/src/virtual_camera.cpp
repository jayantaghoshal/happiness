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

VirtualCamera::VirtualCamera(const sp<IEvsVideoProvider>& input_stream)
    : input_stream_(std::move(input_stream)),
      frames_allowed_(kDefaultFramesAllowed),
      output_stream_state_(StreamState::STOPPED) {}

VirtualCamera::~VirtualCamera() {
    ShutDown();
}

void VirtualCamera::ShutDown() {
    dbgD("called");
    // In normal operation, the stream should already be stopped by the time we get here
    if (output_stream_state_ != StreamState::STOPPED) {
        // Note that if we hit this case, no terminating frame will be sent to the client,
        // but they're probably already dead anyway.
        dbgW("Virtual camera being shutdown while stream is running");
        output_stream_state_ = StreamState::STOPPED;

        if (!frames_held_.empty()) {
            dbgW("VirtualCamera destructing with frames in flight.");
            // Return to the underlying hardware camera any buffers the client was holding
            for (auto&& held_frame : frames_held_) {
                // Tell our parent that we're done with this buffer
                input_stream_->DoneWithFrame(held_frame);
            }
            frames_held_.clear();
        }
    }

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
        return false;  // We do not hold the buffer, so return false;
    }

    if (frames_held_.size() >= frames_allowed_) {
        // TODO(ihu) Consider downgrading to dbgD after evaluation of full EVS stack
        dbgW("Rejecting new frame as we hold %zu of %" PRIu32 " allowed.", frames_held_.size(), frames_allowed_);
        return false;
    }

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
    // If we do not need more buffers.
    if (buffer_count < frames_allowed_) {
        bool result = input_stream_->ChangeFramesInFlight(0);  // Will shrink held buffers to fit.
        if (!result) {
            dbgE("Error when trying to decrease buffer count.");
            // It should always be possible to reduce frames in flight.
            return EvsResult::UNDERLYING_SERVICE_ERROR;
        }
    }
    // If we need more buffers we need to check if they are available first.
    else {
        uint32_t extra_frames = buffer_count - frames_allowed_;
        dbgD("Ask our parent for more buffers");
        // Ask our parent for more buffers
        bool result = input_stream_->ChangeFramesInFlight(extra_frames);
        if (!result) {
            dbgE("Failed to increase buffer count by %" PRIu32 " to %" PRIu32, extra_frames, buffer_count);
            return EvsResult::BUFFER_NOT_AVAILABLE;
        }
    }
    // Update our notion of how many frames we're allowed
    frames_allowed_ = buffer_count;

    return EvsResult::OK;
}

Return<EvsResult> VirtualCamera::startVideoStream(const sp<IEvsCameraStream>& stream) {
    dbgD("called");
    // Each VirtualCamera supports one stream at a time.
    if (output_stream_state_ != StreamState::STOPPED) {
        dbgE("ignoring startVideoStream call since a stream is already running.");
        return EvsResult::STREAM_ALREADY_RUNNING;
    }

    // TODO(ihu) See if this check is actually neccessary.
    // Ensure that we hold no frames since before
    if (!frames_held_.empty()) {
        dbgW("Record of held frames was not empty, returning held frames before proceeding.");
        for (auto&& held_frame : frames_held_) {
            // Tell our parent that we're done with this buffer
            input_stream_->DoneWithFrame(held_frame);
        }
        frames_held_.clear();
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