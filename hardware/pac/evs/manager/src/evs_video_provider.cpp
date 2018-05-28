/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "evs_video_provider.h"

#include <cinttypes>

#include <libdbg.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

namespace {
const char* GetEvsResultAsCString(EvsResult result) {
    switch (result) {
        case EvsResult::BUFFER_NOT_AVAILABLE:
            return "EvsResult::BUFFER_NOT_AVAILABLE";
        case EvsResult::OWNERSHIP_LOST:
            return "EvsResult::OWNERSHIP_LOST";
        case EvsResult::INVALID_ARG:
            return "EvsResult::INVALID_ARG";
        default:
            return "unexpected EvsResult";
    }
}
}  // namespace

EvsVideoProvider::EvsVideoProvider(sp<IEvsCamera>&& hw_camera)
    : hw_camera_(std::move(hw_camera)), output_stream_state_(StreamState::STOPPED) {}

sp<IVirtualCamera> EvsVideoProvider::MakeVirtualCamera() {
    dbgD("called");

    // Create the EVS camera interface object
    sp<IVirtualCamera> client = new VirtualCamera(this);
    if (client == nullptr) {
        dbgE("Failed to create VirtualCamera object, returning nullpointer.");
        return nullptr;
    }

    // Ensure we have enough buffers for all of our clients
    dbgD("Ensure we have enough buffers available for all of our clients");
    if (!ChangeFramesInFlight(client->GetAllowedBuffers())) {
        dbgE("Could not get enough buffers to support the client. dropping our reference, thus destroying the client "
             "object.");
        // Drop our (strong) reference to the client object, thus destroying it.
        client = nullptr;
        return nullptr;
    }

    // Add client to ownership list as a weak pointer
    clients_.emplace_back(client);

    // Return the strong pointer to the client
    return client;
}

void EvsVideoProvider::DisownVirtualCamera(sp<IVirtualCamera>& virtual_camera) {
    dbgD("called");
    // Validate input
    if (virtual_camera == nullptr) {
        dbgE("Input virtual_camera was nullptr. Ignoring call.");
        return;
    }

    // Ensure the virtual_camera:s stream is stopped
    virtual_camera->stopVideoStream();

    // Remove the virtual_camera from our list of clients
    auto client_count_pre_remove = GetClientCount();
    clients_.remove(virtual_camera);
    if (GetClientCount() == client_count_pre_remove) {
        dbgE("Virtual camera to be removed was not present in list of clients.");
    }
    virtual_camera->ShutDown();
    virtual_camera = nullptr;

    // Shrink-to-fit the record of held frames
    if (!ChangeFramesInFlight(0)) {
        dbgE("Error when trying to reduce the in flight buffer count");
    }
}

Return<EvsResult> EvsVideoProvider::RequestVideoStream() {
    dbgD("called");
    // A client is requesting video, ensure that the hardware camera stream is running.
    // If the hardware camera stream is already running, return OK.
    // Note that we do not use StreamState::STOPPING in evs_video_provider.
    if (output_stream_state_ == StreamState::RUNNING) {
        return EvsResult::OK;
    }

    // Otherwise try to start the hardware camera stream.
    Return<EvsResult> result = hw_camera_->startVideoStream(this);
    if (result.isOk() && result == EvsResult::OK) {
        output_stream_state_ = StreamState::RUNNING;
    }

    return result;
}

void EvsVideoProvider::ReleaseVideoStream() {
    dbgD("called");
    // A client no longer needs video, it may be time to stop the hardware camera stream
    // Check for streaming clients
    bool is_any_client_streaming = false;
    sp<IVirtualCamera> vcam;
    for (auto&& client : clients_) {
        vcam = client.promote();
        if (vcam != nullptr && vcam->IsStreaming()) {
            is_any_client_streaming = true;
            break;
        }
    }

    // If no client is streaming, stop video stream from hardware camera
    if (!is_any_client_streaming) {
        hw_camera_->stopVideoStream();
        output_stream_state_ = StreamState::STOPPED;
    }
}

void EvsVideoProvider::DoneWithFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32,
         buffer.memHandle != nullptr ? buffer.bufferId : 0);  // Use 0 if buffer is empty

    if (buffer.memHandle == nullptr) {
        dbgE("called with buffer.memHandle == nullptr. Ignoring call.");
        return;
    }

    // Find frame in list of outstanding frames
    size_type_frames i;
    for (i = 0; i < frames_.size(); i++) {
        if (frames_[i].frame_id == buffer.bufferId) {
            break;
        }
    }

    if (i == frames_.size()) {
        dbgE("called with unrecognized bufferId %" PRIu32 ". Ignoring call.", buffer.bufferId);
        return;
    }

    // Check if other clients are using the buffer
    frames_[i].ref_count--;
    if (frames_[i].ref_count <= 0) {
        // No client is using the buffer, return it to the device layer
        hw_camera_->doneWithFrame(buffer);
    }
}

bool EvsVideoProvider::ChangeFramesInFlight(uint32_t extra_frames) {
    // Count the sum of frames currently required by clients
    uint32_t buffer_count = 0;
    sp<IVirtualCamera> vcam;
    for (auto&& client : clients_) {
        vcam = client.promote();
        if (vcam != nullptr) {
            dbgD("Adding %" PRIu32 " buffers to buffer_count(%" PRIu32 ")", vcam->GetAllowedBuffers(), buffer_count);
            buffer_count += vcam->GetAllowedBuffers();
            dbgD("Added %" PRIu32 " buffers to buffer_count that now is %" PRIu32 " buffers in total",
                 vcam->GetAllowedBuffers(),
                 buffer_count);
        }
    }

    uint32_t desired_buffer_count = buffer_count + extra_frames;
    // Check sum for uint overflow
    if (desired_buffer_count < buffer_count || desired_buffer_count < extra_frames) {
        dbgD("Overflow in desired_buffer_count, total buffer_count was %" PRIu32 " and input extra_frames was %" PRIu32,
             buffer_count,
             extra_frames);
        return false;
    }

    // Never drop below 1 buffer -- even if all client cameras are closed
    if (desired_buffer_count < 1) {
        desired_buffer_count = 1;
    }

    dbgD("Ask the hardware for the resulting buffer count");
    // Ask the hardware for the resulting buffer count
    dbgD("Try hw_camera_->setMaxFramesInFlight(%" PRIu32 " == desired_buffer_count)", desired_buffer_count);
    Return<EvsResult> result = hw_camera_->setMaxFramesInFlight(desired_buffer_count);

    // If we can not provide the desired buffer count, return false.
    if (!(result.isOk() && result == EvsResult::OK)) {
        dbgD("called hw_camera_->setMaxFramesInFlight(%" PRIu32 " ) got %s, returning false",
             desired_buffer_count,
             GetEvsResultAsCString(result));
        return false;
    }

    dbgD("called hw_camera_->setMaxFramesInFlight(%" PRIu32 " ) got EvsResult::OK, proceeding", desired_buffer_count);

    // Cast desired_buffer_count to size_type_frames.
    static_assert(std::numeric_limits<size_type_frames>::max() >= UINT32_MAX,
                  "size_type_frames must be able to contain desired_buffer_count");
    size_type_frames requested_buffer_count = static_cast<size_type_frames>(desired_buffer_count);

    // Update the size of our array of outstanding frame records
    std::vector<FrameRecord> new_frames;
    new_frames.reserve(requested_buffer_count);

    // Copy and compact the old records that are still active
    for (const auto& rec : frames_) {
        if (rec.ref_count > 0) {
            new_frames.emplace_back(rec);
        }
    }
    if (new_frames.size() > requested_buffer_count) {
        dbgW("We found more frames in use than requested.");
    }

    frames_.swap(new_frames);
    return true;
}

Return<void> EvsVideoProvider::deliverFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32,
         buffer.memHandle != nullptr ? buffer.bufferId : 0);  // Use 0 if buffer is empty

    // Deliver frame to any eligible client
    unsigned int frame_deliveries = 0;
    sp<IVirtualCamera> vcam;
    for (auto&& client : clients_) {
        vcam = client.promote();
        if (vcam != nullptr && vcam->DeliverFrame(buffer)) {
            frame_deliveries++;
        }
    }

    // If no client could accept the frame, return it
    if (frame_deliveries == 0) {
        dbgI("Trivially rejecting frame with no acceptances");  // TODO(ihu) consider downgrading to debug
        hw_camera_->doneWithFrame(buffer);
        return Void();
    }

    // Add an entry for the frame in the tracking list
    // If the list has an unused entry (ref_count == 0) replace it, else create a new entry.
    size_type_frames i;
    for (i = 0; i < frames_.size(); i++) {
        if (frames_[i].ref_count == 0) {
            break;
        }
    }
    if (i == frames_.size()) {
        frames_.emplace_back(buffer.bufferId);
    } else {
        frames_[i].frame_id = buffer.bufferId;
    }
    frames_[i].ref_count = frame_deliveries;

    return Void();
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android