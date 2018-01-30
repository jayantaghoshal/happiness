# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)


LOCAL_SRC_FILES += $(call all-java-files-under, aidl)
LOCAL_SRC_FILES += $(call all-java-files-under, src)
LOCAL_SRC_FILES += $(call all-java-files-under, test)
LOCAL_SRC_FILES += aidl/com/volvocars/userswitch/IUserSwitchService.aidl
LOCAL_AIDL_INCLUDES += $(LOCAL_PATH)/aidl
LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res

LOCAL_PACKAGE_NAME := VccUserSwitchService
LOCAL_MANIFEST_FILE := AndroidManifest.xml
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_USE_AAPT2 := true

LOCAL_PROGUARD_ENABLED := disabled

LOCAL_STATIC_JAVA_LIBRARIES += services.core \
        android.hidl.manager-V1.0-java \
        vendor.volvocars.hardware.profiles-V1.0-java-static

include $(BUILD_PACKAGE)

