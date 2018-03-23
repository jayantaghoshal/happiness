# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := MicrophoneTestApp
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, /src/main/java)
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_RESOURCE_DIR += $(LOCAL_PATH)/src/main/res

LOCAL_PROGUARD_ENABLED := disabled
LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_STATIC_JAVA_LIBRARIES := \
	ctstestrunner \
	android-support-test \
	junit \

LOCAL_COMPATIBILITY_SUITE := cts
include $(BUILD_CTS_PACKAGE)




