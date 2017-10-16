LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_VENDOR_MODULE := true

LOCAL_MODULE := VtsPacketInjectorCTTestCases

LOCAL_SRC_FILES := \
    src/main.cpp \
	src/LscMocker.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_STATIC_LIBRARIES += \
    libpacketinjector_static

LOCAL_SHARED_LIBRARIES += \
    liblog \
    libtarmac \
    libutils \
    libipcommandbus \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    vendor.volvocars.hardware.iplm@1.0

#LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
#LOCAL_COMPATIBILITY_SUITE := vts

LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)
