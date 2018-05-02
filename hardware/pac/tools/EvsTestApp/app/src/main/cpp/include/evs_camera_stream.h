/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsCameraStream.h>
#include <android/hardware/automotive/evs/1.0/IEvsDisplay.h>
#include <android/hardware/automotive/evs/1.0/IEvsEnumerator.h>

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

using ::android::hardware::automotive::evs::V1_0::BufferDesc;
using ::android::hardware::automotive::evs::V1_0::IEvsCamera;
using ::android::hardware::automotive::evs::V1_0::IEvsCameraStream;
using ::android::hardware::automotive::evs::V1_0::IEvsEnumerator;

// Manages the communication with the EVS service by giving a green light to
// the video_tex object when a new frame is available and to the service when a frame
// has been used so that it can be destroyed and make room for a new frame.
class EvsCameraStream : public IEvsCameraStream {
  public:
    ~EvsCameraStream();
    const char* StartCameraStream();

    bool NewFrameAvailable();

    // Tells the EVS camera that the frame described by the buffer_desc argument
    // can be dropped to make room for a new frame.
    void DoneWithFrame(BufferDesc& buffer_desc);

    BufferDesc buffer_desc_ = {};

  private:
    u_int8_t current_frame_amount_ = 0;

    sp<IEvsEnumerator> evs_enumerator_;
    sp<IEvsCamera> camera_;

    // The callback function used by the camera object in the EVS service.
    // Contains all the necessary information to retrieve a video frame buffer.
    Return<void> deliverFrame(const BufferDesc& buffer) override;
};