# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)
#java
include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_PACKAGE_NAME := cloudtest
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
ANDROID_PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)
LOCAL_STATIC_JAVA_LIBRARIES := ctstestrunner \
                                    junit \
                                    android-support-test \
                                    vendor.volvocars.fslib
LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_COMPATIBILITY_SUITE := cts

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64
include $(BUILD_CTS_PACKAGE)