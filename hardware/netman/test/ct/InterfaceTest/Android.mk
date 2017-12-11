# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := networkInterfaceTest
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
ANDROID_PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)
LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_STATIC_JAVA_LIBRARIES := \
	ctstestrunner \
	android-support-test \
	junit

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_COMPATIBILITY_SUITE := cts

include $(BUILD_CTS_PACKAGE)
