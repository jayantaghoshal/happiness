/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "evs_camera_stream.h"

// Manages the creation of an image which can be used and bound as a texture.
// The texture consists of a video frame captured by the camera. Every time a
// new frame is made available, a new image needs to be created and bound to the
// the same texture slot.
class VideoTex {
  public:
    VideoTex(EvsCameraStream* evs_camera_stream, GLuint gl_id) : evs_camera_stream_(evs_camera_stream), gl_id_(gl_id) {}

    // Fetches the most recent camera buffer, a BufferDesc object, set in the callback method deliverFrame
    // as defined in the EvsCameraStream class and creates an image out of it, which in turn is used for texturing.
    const char* RefreshVideoTex();

  private:
    EvsCameraStream* evs_camera_stream_;
    GLuint gl_id_;
    EGLImageKHR KHR_image_ = EGL_NO_IMAGE_KHR;
};
