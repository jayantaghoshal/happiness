# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(call all-java-files-under, src) \
    $(call all-java-files-under, ../../../services)

LOCAL_PACKAGE_NAME := settingsstorageserviceut

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_INSTRUMENTATION_FOR := SettingsStorageService

LOCAL_STATIC_JAVA_LIBRARIES += \
    junit \
    android-support-test \
    mockito-target-minus-junit4 \
    vendor.volvocars.hardware.settings-V1.0-java \
    com.volvocars.settingsstorageservice.ISettingsStorageManager \

LOCAL_JAVA_LIBRARIES := \
    android.test.runner


LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)
