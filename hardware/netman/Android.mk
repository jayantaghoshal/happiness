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

include $(CLEAR_VARS)
LOCAL_MODULE := netboyd.rules
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := conf/netboy.rules
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)
#
# Copy /system/bin/ip to /vendor/bin/ip
#
include $(CLEAR_VARS)
LOCAL_MODULE := ip-vcc
LOCAL_REQUIRED_MODULES := ip
LOCAL_POST_INSTALL_CMD := $(hide) cp $(PRODUCT_OUT)/system/bin/ip $(TARGET_OUT_VENDOR)/bin/ip
include $(BUILD_PHONY_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))
