# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := com.volvocars.softwareupdate.ISoftwareUpdateManager
LOCAL_STATIC_JAVA_LIBRARIES += \
    com.volvocars.cloudservice.ISoftwareManagementApi \

LOCAL_SRC_FILES := $(call all-java-files-under, src) \
src/com/volvocars/softwareupdate/ISoftwareUpdateManager.aidl \
src/com/volvocars/softwareupdate/ISoftwareUpdateManagerCallback.aidl \
src/com/volvocars/softwareupdate/ISoftwareUpdateSettingsCallback.aidl

LOCAL_AIDL_INCLUDES := src/com/volvocars/cloudservice/SoftwareAssignment.aidl \
src/com/volvocars/cloudservice/InstallationOrder.aidl \
src/com/volvocars/cloudservice/Query.aidl \

include $(BUILD_STATIC_JAVA_LIBRARY)
