# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_PACKAGE_NAME := VtsKeyPanelTestApk
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64
include $(BUILD_PACKAGE)

include $(CLEAR_VARS)
LOCAL_MODULE := VtsKeyPanelTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/test/keypanel/
include test/vts/tools/build/Android.host_config.mk
