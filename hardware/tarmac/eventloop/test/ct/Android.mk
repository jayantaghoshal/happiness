LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := VtsEventLoopCTTestCases

LOCAL_SRC_FILES := \
    eventloop_ct_test.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_STATIC_LIBRARIES += \
    libtarmac_static

LOCAL_SHARED_LIBRARIES += liblog

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

include $(BUILD_NATIVE_TEST)
