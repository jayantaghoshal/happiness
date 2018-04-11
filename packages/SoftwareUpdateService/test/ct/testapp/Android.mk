# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(call all-java-files-under, src) \
    $(call all-java-files-under, test)

LOCAL_PACKAGE_NAME := softwareupdateapp

LOCAL_MODULE_TAGS := optional

LOCAL_USE_AAPT2 := true

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_STATIC_JAVA_LIBRARIES += \
    ctstestrunner \
    com.volvocars.SoftwareUpdateLib \
    junit \
    android-support-test \
    ub-uiautomator \
    android-support-v4 \
    android-support-v7-appcompat \
    android-support-design \
    android-support-v7-cardview \
    android-support-v7-recyclerview \
    android-support-v13 \
    android-support-transition

LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_AAPT_FLAGS := \
--auto-add-overlay \
--extra-packages android.support.v7.appcompat \
--extra-packages android.support.design \
--extra-packages android.support.v7.cardview \
--extra-packages android.support.v7.recyclerview

LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res \
frameworks/support/v7/appcompat/res \
frameworks/support/design/res \
frameworks/support/v7/cardview/res \
frameworks/support/v7/recyclerview/res

LOCAL_COMPATIBILITY_SUITE := cts

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64

include $(BUILD_CTS_PACKAGE)
