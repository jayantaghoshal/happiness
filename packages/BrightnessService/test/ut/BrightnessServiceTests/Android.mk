# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_STATIC_JAVA_LIBRARIES := ctstestrunner \
                               junit \
                               mockito-target \
                               android-support-test \

LOCAL_JAVA_LIBRARIES := android.test.runner
LOCAL_PACKAGE_NAME := brightnessserviceunittest
LOCAL_INSTRUMENTATION_FOR := BrightnessService

LOCAL_COMPATIBILITY_SUITE := cts
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true

include $(BUILD_CTS_PACKAGE)
