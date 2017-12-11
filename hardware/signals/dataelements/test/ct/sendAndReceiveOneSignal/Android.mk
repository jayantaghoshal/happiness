# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#Build python test
LOCAL_MODULE := VtsFlexraySignalingCT_OneSendOneReceive
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/signals/dataelements/test/ct/sendAndReceiveOneSignal
include test/vts/tools/build/Android.host_config.mk




#Build flexray_test c++ binary
include $(CLEAR_VARS)
LOCAL_MODULE := flexray_test

LOCAL_SRC_FILES := flexray_test.cpp

LOCAL_SHARED_LIBRARIES :=   liblog \
                            libc++ \
                            libdataelements

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor \
                  -fexceptions \
                  -Wno-unused-parameter \
                  -Wno-macro-redefined

# We only build for 64 bit.
LOCAL_MULTILIB := 64

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

include $(BUILD_EXECUTABLE)



