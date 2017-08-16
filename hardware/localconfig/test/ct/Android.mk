LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := VtsLocalConfigCTTestCases

LOCAL_SRC_FILES := \
    localconfig_ct_test.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_SHARED_LIBRARIES += \
    liblocalconfig

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit architecture
# "first" refers to the first target architecture set by lunch
LOCAL_MULTILIB := first
LOCAL_MODULE_STEM_64 := $(LOCAL_MODULE)64

include $(BUILD_NATIVE_TEST)
