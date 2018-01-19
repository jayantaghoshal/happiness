# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Build the VendorVCC priv app.
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_PACKAGE_NAME := CloudApp

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_STATIC_JAVA_LIBRARIES += \
    vendor.volvocars.hardware.cloud-V1.0-java \
    vendor.volvocars.cloudservice.IFoundationServicesApi \
    vendor.volvocars.cloudservice.ISoftwareManagementApi \

LOCAL_MODULE_TAGS := optional
LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))