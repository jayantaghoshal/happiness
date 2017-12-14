# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

#
# Copy the default carconfig file to /vendor/etc/config
#
include $(CLEAR_VARS)
LOCAL_MODULE := carconfig-default.csv
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_RELATIVE_PATH := config
LOCAL_SRC_FILES := carconfig-default.csv
# This is probably not needed, please review...
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)
include $(call all-makefiles-under,$(LOCAL_PATH))