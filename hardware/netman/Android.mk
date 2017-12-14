# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

#
# Copy sysctl.conf to /vendor/etc/
#
include $(CLEAR_VARS)
LOCAL_MODULE := sysctl.conf
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := conf/sysctl.conf
# This is probably not needed, please review...
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)

#
# Copy conf/netd_config.xml to /system/etc/netd
#
include $(CLEAR_VARS)
LOCAL_MODULE := netd_config.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_RELATIVE_PATH := netd
LOCAL_SRC_FILES := conf/netd_config.xml
include $(BUILD_PREBUILT)

include $(call all-makefiles-under,$(LOCAL_PATH))
