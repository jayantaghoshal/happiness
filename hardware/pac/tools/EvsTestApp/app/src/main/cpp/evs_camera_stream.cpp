/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "evs_camera_stream.h"

#include <libdbg.h>

using ::android::hardware::automotive::evs::V1_0::EvsResult;

EvsCameraStream::~EvsCameraStream() {
    camera_->stopVideoStream();

    evs_enumerator_->closeCamera(camera_);
}

const char* EvsCameraStream::StartCameraStream() {
    evs_enumerator_ = IEvsEnumerator::getService("EvsEnumeratorHw");

    if (evs_enumerator_ == nullptr) {
        return "Service not connected";
    }

    camera_ = evs_enumerator_->openCamera("/dev/video32");

    if (camera_ == nullptr) {
        return "No camera!";
    }

    camera_->setMaxFramesInFlight(2);

    Return<EvsResult> stream_result = camera_->startVideoStream(this);

    if (stream_result != EvsResult::OK) {
        return "Video stream couldn't be started";
    }

    return "Success";
}

void EvsCameraStream::DoneWithFrame(BufferDesc& buffer_desc) {
    camera_->doneWithFrame(buffer_desc);
    current_frame_amount_--;
}

Return<void> EvsCameraStream::deliverFrame(const BufferDesc& buffer) {
    buffer_desc_ = buffer;

    if (buffer_desc_.memHandle.getNativeHandle()->data[0] == 0) {
        dbgD("Copy buffer failed");
    }

    current_frame_amount_++;

    return Void();
}

bool EvsCameraStream::NewFrameAvailable() { return current_frame_amount_ > 0; }