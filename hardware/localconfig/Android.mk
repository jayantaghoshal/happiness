# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

# Copy the default JSON file to /vendor/etc/localconfig
# Convert to BP when somepne figures that out
include $(CLEAR_VARS)
LOCAL_MODULE := localconfig.json
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_RELATIVE_PATH := localconfig
LOCAL_SRC_FILES := misc/localconfig.json

include $(BUILD_PREBUILT)
include $(call all-makefiles-under,$(LOCAL_PATH))
