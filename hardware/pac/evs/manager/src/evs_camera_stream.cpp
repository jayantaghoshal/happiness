/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "evs_camera_stream.h"

#include <libdbg.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

sp<VirtualCamera> EvsCameraStream::MakeVirtualCamera() {
    dbgD("called");

    // Create the EVS camera interface object
    sp<VirtualCamera> client = new VirtualCamera(this);
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

void EvsCameraStream::DisownVirtualCamera(const sp<VirtualCamera>& virtual_camera) {
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

Return<void> EvsCameraStream::deliverFrame(const BufferDesc& buffer) {
    dbgD("called with bufferId %u", buffer.bufferId);
    // TODO(ihu) Implement EvsCameraStream::deliverFrame method
    return Void();
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android