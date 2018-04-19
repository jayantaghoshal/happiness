# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := VtsRtpAudioNamespaceBridge

LOCAL_SRC_FILES := \
    rtp_audio_namespace_bridge.cpp

LOCAL_STATIC_LIBRARIES := \
    libgmock \

LOCAL_SHARED_LIBRARIES += \
    liblog \

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain \
    liblog

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)
