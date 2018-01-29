# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := VtsApixObdTrafficSplitIntegrationTest

LOCAL_SRC_FILES := \
    apix_obd_traffic_split.cpp

LOCAL_C_INCLUDES := external/iptables/include \

LOCAL_STATIC_LIBRARIES := \
    libgmock \
	libip4tc \


LOCAL_SHARED_LIBRARIES += \
    liblog \
    libhidlbase \
    vendor.volvocars.hardware.uds@1.0\
    libutils \

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain \
    liblog

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)
