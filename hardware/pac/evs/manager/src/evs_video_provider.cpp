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

sp<IVirtualCamera> EvsVideoProvider::MakeVirtualCamera() {
    dbgD("called");

    // Create the EVS camera interface object
    sp<IVirtualCamera> client = new VirtualCamera(this);
    if (client == nullptr) {
        dbgE("Failed to create VirtualCamera object, returning nullpointer.");
        return nullptr;
    }

    // TODO(ihu) Ensure we have enough buffers

    // Add client to ownership list as a weak pointer
    clients_.emplace_back(client);

    // Return the strong pointer to the client
    return client;
}

void EvsVideoProvider::DisownVirtualCamera(const sp<IVirtualCamera>& virtual_camera) {
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
    virtual_camera->Shutdown();

    // TODO(ihu) Recalculate the number of buffers required with the client removed from the list.
}

Return<EvsResult> EvsVideoProvider::RequestVideoStream() {
    dbgD("called");
    // A client is requesting video, ensure that the hardware camera stream is running.
    // If the hardware camera stream is already running, return OK.
    // TODO(ihu) Revisit this and consider how to handle "STOPPING" state.
    if (output_stream_state_ != StreamState::STOPPED) {
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
    unsigned int i;
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

Return<void> EvsVideoProvider::deliverFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32,
         buffer.memHandle != nullptr ? buffer.bufferId : 0);  // Use 0 if buffer is empty

    // Deliver frame to any eligible client
    unsigned int frame_deliveries = 0;
    for (auto&& client : clients_) {
        sp<IVirtualCamera> cam_client = client.promote();
        if (cam_client != nullptr && cam_client->DeliverFrame(buffer)) {
            frame_deliveries++;
        }
    }

    if (frame_deliveries == 0) {
        // If no client could accept the frame, return it
        dbgI("Trivially rejecting frame with no acceptances");  // TODO(ihu) consider downgrading to debug
        hw_camera_->doneWithFrame(buffer);
        return Void();
    }

    // Add an entry for the frame in the tracking list
    // If the list has an unused entry (ref_count == 0) replace it, else create a new entry.
    size_t i;
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