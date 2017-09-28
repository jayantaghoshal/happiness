LOCAL_PATH := $(call my-dir)

MY_LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

include $(CLEAR_VARS)

#Build python test
LOCAL_MODULE := VtsNetmanCT_NetdBlacklistTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/netman/test/ct/netd_blacklist_test
include test/vts/tools/build/Android.host_config.mk


# Target executable for relaying notifications from netd to host
include $(CLEAR_VARS)
LOCAL_MODULE := netd_blacklist_test
LOCAL_SRC_FILES := \
	netd_blacklist_test.cpp
LOCAL_CPPFLAGS := $(MY_LOCAL_CPPFLAGS)
LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_EXPORT_C_INCLUDE_DIRS)
LOCAL_SHARED_LIBRARIES += \
	liblog \
	libcutils
LOCAL_STATIC_LIBRARIES += \
	libbase

# We only build for 64 bit.
LOCAL_MULTILIB := 64

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := netd_blacklist_config_blacklist
LOCAL_SRC_FILES := config/netd_blacklist_config_blacklist.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_SUFFIX := .xml
LOCAL_MULTILIB := 64
LOCAL_COMPATIBILITY_SUITE := vts
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := netd_blacklist_config_empty_blacklist
LOCAL_SRC_FILES := config/netd_blacklist_config_empty_blacklist.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_SUFFIX := .xml
LOCAL_MULTILIB := 64
LOCAL_COMPATIBILITY_SUITE := vts
include $(BUILD_PREBUILT)
