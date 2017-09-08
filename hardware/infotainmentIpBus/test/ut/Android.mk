LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/../Android.mk

LOCAL_MODULE := VtsLibInfotainmentIpBusTests

LOCAL_SRC_FILES := \
    testASN1.cpp

LOCAL_STATIC_LIBRARIES += \
    libinfotainmentbusasn_static

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)
