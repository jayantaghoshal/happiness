LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := localconfig_test_gnss
LOCAL_SRC_FILES := localconfig.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MULTILIB := 64
LOCAL_COMPATIBILITY_SUITE := vts
include $(BUILD_PREBUILT)


include $(CLEAR_VARS)
LOCAL_MODULE := VtsGnssIntegrationTest

LOCAL_SRC_FILES := \
    VtsGnssIntegrationTest.cpp

LOCAL_STATIC_LIBRARIES += \
    libipcb_simulator_static \
    libinfotainmentbusasn_static


LOCAL_SHARED_LIBRARIES += \
    liblog \
    liblocalconfig \
    libutils \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    libtarmac \
    libipcommandbus \
    android.hardware.gnss@1.0



LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain \
    liblog

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)
