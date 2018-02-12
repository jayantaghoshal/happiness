# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := com.volvocars.cloudservice.ISoftwareManagementApi
LOCAL_SRC_FILES := $(call all-java-files-under, src) \
src/com/volvocars/cloudservice/ISoftwareManagementApi.aidl \
src/com/volvocars/cloudservice/ISoftwareManagementApiCallback.aidl

LOCAL_AIDL_INCLUDES := src/com/volvocars/cloudservice/SofwareAssignment.aidl \
src/com/volvocars/cloudservice/InstallationOrder.aidl

include $(BUILD_STATIC_JAVA_LIBRARY)
