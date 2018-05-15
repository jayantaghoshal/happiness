/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "video_tex.h"

#include <ui/GraphicBuffer.h>

using ::android::GraphicBuffer;

const char* VideoTex::RefreshVideoTex() {
    // If nothing else the already retrieved texture has to be bound
    // for the draw call to be made just after this function has been called
    // in the java part of the application
    glBindTexture(GL_TEXTURE_2D, gl_id_);

    if (!evs_camera_stream_->NewFrameAvailable()) {
        return "No new frames available";
    }

    BufferDesc buffer_desc = evs_camera_stream_->buffer_desc_;

    sp<GraphicBuffer> gfx_buffer = new GraphicBuffer(buffer_desc.memHandle,
                                                     GraphicBuffer::CLONE_HANDLE,
                                                     buffer_desc.width,
                                                     buffer_desc.height,
                                                     buffer_desc.format,
                                                     1,
                                                     GRALLOC_USAGE_HW_TEXTURE,
                                                     buffer_desc.stride);

    if (gfx_buffer.get() == nullptr) {
        return "No graphics-buffer allocated";
    }

    EGLDisplay egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (egl_display == EGL_NO_DISPLAY) {
        return "No display created";
    }

    EGLint egl_image_attributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    EGLClientBuffer client_buf = static_cast<EGLClientBuffer>(gfx_buffer->getNativeBuffer());

    KHR_image_ =
            eglCreateImageKHR(egl_display, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, client_buf, egl_image_attributes);

    if (KHR_image_ != EGL_NO_IMAGE_KHR) {
        glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, static_cast<GLeglImageOES>(KHR_image_));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    evs_camera_stream_->DoneWithFrame(buffer_desc);

    eglDestroyImageKHR(egl_display, KHR_image_);
    KHR_image_ = EGL_NO_IMAGE_KHR;

    return "Success";
}