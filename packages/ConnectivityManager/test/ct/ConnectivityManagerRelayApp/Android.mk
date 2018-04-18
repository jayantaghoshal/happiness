# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(call all-java-files-under, test)

LOCAL_PACKAGE_NAME := connectivitymanager.relay.test

LOCAL_MODULE_TAGS := optional

LOCAL_USE_AAPT2 := true

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_STATIC_JAVA_LIBRARIES += \
    ctstestrunner \
    junit \
    android-support-test \
    ub-uiautomator \

LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_AAPT_FLAGS := \
--auto-add-overlay \

#LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res \

LOCAL_COMPATIBILITY_SUITE := cts

LOCAL_CERTIFICATE := platform
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64

include $(BUILD_CTS_PACKAGE)
