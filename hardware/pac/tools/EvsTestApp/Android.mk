# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := VccEvsTestApp
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_TAGS := optional
LOCAL_MIN_SDK_VERSION := 26
LOCAL_PRIVATE_PLATFORM_APIS := true

LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml

LOCAL_USE_AAPT2 := true

LOCAL_SRC_FILES := $(call all-java-files-under, app/src/main/java)

LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/app/src/main/res

LOCAL_JNI_SHARED_LIBRARIES := libevsnative

LOCAL_STATIC_ANDROID_LIBRARIES := android-support-design

include $(BUILD_PACKAGE)
