# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vsomeip_convapi.json
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := vsomeip
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := conf/vsomeip_convapi.json
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)
