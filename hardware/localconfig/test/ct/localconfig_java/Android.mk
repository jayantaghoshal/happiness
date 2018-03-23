# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under,src)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_STATIC_JAVA_LIBRARIES := ctstestrunner \
                               android.volvocars.localconfig \
                               junit \
                               android-support-test

LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_PACKAGE_NAME := LocalConfigTest

LOCAL_COMPATIBILITY_SUITE := cts
# for system|priviledged permission.
LOCAL_CERTIFICATE := platform

LOCAL_PRIVILEGED_MODULE := true

LOCAL_MULTILIB := 64

include $(BUILD_CTS_PACKAGE)
