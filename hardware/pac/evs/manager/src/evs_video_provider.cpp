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
        dbgE("Input virtual_camera was nullptr.");
        return;
    }

    // Ensure the virtual_camera:s stream is stopped
    virtual_camera->stopVideoStream();

    // Remove the virtual_camera from our list of clients
    auto client_count_pre_remove = GetClientCount();
    clients_.remove(virtual_camera);
    if (GetClientCount() == client_count_pre_remove) {
        dbgE("EVS camera client to be removed was not present in list of clients.");
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

Return<void> EvsVideoProvider::deliverFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %" PRIu32,
         buffer.memHandle != nullptr ? buffer.bufferId : 0);  // Use 0 if buffer is empty
    // TODO(ihu) Implement EvsVideoProvider::deliverFrame method
    return Void();
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android