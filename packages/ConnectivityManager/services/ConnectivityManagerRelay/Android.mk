# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_SRC_FILES += $(call all-Iaidl-files-under,aidl)
LOCAL_AIDL_INCLUDES := $(LOCAL_PATH)/aidl

LOCAL_PACKAGE_NAME := ConManRelay

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_MODULE_TAGS := optional
LOCAL_MULTILIB := 64

LOCAL_STATIC_JAVA_LIBRARIES += \
    android-support-v7-appcompat \
    com.volvocars.ConnectivityManagerLib \
    com.volvocars.connectivitymanager.IConnectivityManager \

LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res \
    frameworks/support/v7/appcompat/res \

LOCAL_USE_AAPT2 := true

LOCAL_AAPT_FLAGS := \
--auto-add-overlay \
--extra-packages android.support.v7.appcompat \

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))