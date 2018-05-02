/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <GLES2/gl2.h>
#include <jni.h>

#include "evs_camera_stream.h"
#include "video_tex.h"

namespace {

// Calls the method which connects with and manages the EVS service.
// Contains the callback method which is used by the IEvsCamera to make the
// video frames available through a BufferDesc object.
EvsCameraStream* evs_camera_stream;

// Uses the evs_camera_stream object to access the BufferDesc object to create
// images which are displayed through opengl.
VideoTex* video_tex;

}  // namespace

extern "C" {

JNIEXPORT
jstring JNICALL Java_com_volvocars_evstestapp_NdkManager_initEvsCameraStream(JNIEnv* env) {
    evs_camera_stream = new EvsCameraStream;

    return env->NewStringUTF(evs_camera_stream->StartCameraStream());
}

JNIEXPORT
jstring JNICALL Java_com_volvocars_evstestapp_NdkManager_generateTexture(JNIEnv* env) {
    GLuint gl_id = 0;
    glGenTextures(1, &gl_id);

    if (gl_id == 0) {
        return env->NewStringUTF("Texture not generated");
    }

    video_tex = new VideoTex(evs_camera_stream, gl_id);

    return env->NewStringUTF("Texture generated");
}

JNIEXPORT
jstring JNICALL Java_com_volvocars_evstestapp_NdkManager_refreshVideo(JNIEnv* env) {
    return env->NewStringUTF(video_tex->RefreshVideoTex());
}

JNIEXPORT
jstring JNICALL Java_com_volvocars_evstestapp_NdkManager_closeCameraStream(JNIEnv* env) {
    delete evs_camera_stream;
    delete video_tex;
    return env->NewStringUTF("Camera closed");
}
}