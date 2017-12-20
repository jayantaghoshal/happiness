# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

# VehiclePropertiesSample
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, java)
LOCAL_ASSET_FILES += $(call find-subdir-assets, assets)


LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res

   LOCAL_STATIC_ANDROID_LIBRARIES := \
       android-support-design \
       android-support-transition \
       android-support-v7-appcompat \
       android-support-v7-recyclerview \
       android-support-v4

LOCAL_USE_AAPT2 := true

# Include support-v7-appcompat, if not already included
ifeq (,$(findstring android-support-v7-appcompat,$(LOCAL_STATIC_JAVA_LIBRARIES)))
LOCAL_RESOURCE_DIR += frameworks/support/v7/appcompat/res
LOCAL_AAPT_FLAGS += --extra-packages android.support.v7.appcompat
LOCAL_STATIC_JAVA_LIBRARIES += android-support-v7-appcompat
endif

LOCAL_PACKAGE_NAME := VehiclePropertiesSample

LOCAL_CERTIFICATE := platform

LOCAL_MODULE_TAGS := optional

LOCAL_PRIVILEGED_MODULE := true

LOCAL_JAVA_LIBRARIES := framework

LOCAL_STATIC_JAVA_LIBRARIES += carconfigapi

LOCAL_PROGUARD_ENABLED := disabled

LOCAL_DEX_PREOPT := false

LOCAL_SDK_VERSION := current
LOCAL_MIN_SDK_VERSION := 25

LOCAL_AAPT_FLAGS := \
    --auto-add-overlay \
    --extra-packages android.support.design

include $(BUILD_PACKAGE)

include $(CLEAR_VARS)