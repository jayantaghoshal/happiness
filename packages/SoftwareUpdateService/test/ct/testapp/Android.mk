# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_PACKAGE_NAME := softwareupdateapp
LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
#ANDROID_PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)
LOCAL_STATIC_JAVA_LIBRARIES := com.volvocars.SoftwareUpdateLib \
android-support-v4
LOCAL_STATIC_JAVA_LIBRARIES += android-support-v7-appcompat
LOCAL_STATIC_JAVA_LIBRARIES += android-support-design
LOCAL_STATIC_JAVA_LIBRARIES += android-support-v7-cardview
LOCAL_STATIC_JAVA_LIBRARIES += android-support-v7-recyclerview
LOCAL_STATIC_JAVA_LIBRARIES += android-support-v13

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

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MULTILIB := 64
include $(BUILD_PACKAGE)
