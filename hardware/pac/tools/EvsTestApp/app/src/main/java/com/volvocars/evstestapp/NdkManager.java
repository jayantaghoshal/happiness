/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

class NdkManager {
    // Used to load the native library on application startup.
    static {
        System.loadLibrary("evsnative");
    }

    // The native functions that can be found in libevsnative.so
    public static native String initEvsCameraStream();
    public static native String refreshVideo();
    public static native String closeCameraStream();
    public static native String generateVideoTexture();
}
