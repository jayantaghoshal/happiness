# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_PACKAGE_NAME := softwareupdateapp
LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
#ANDROID_PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)
LOCAL_STATIC_JAVA_LIBRARIES := com.volvocars.SoftwareUpdateLib

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64
include $(BUILD_PACKAGE)
