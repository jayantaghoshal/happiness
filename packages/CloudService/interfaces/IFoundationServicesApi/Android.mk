# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := vendor.volvocars.cloudservice.IFoundationServicesApi
LOCAL_SRC_FILES := $(call all-Iaidl-files-under, src)
LOCAL_SRC_FILES += $(call all-java-files-under, src)
include $(BUILD_STATIC_JAVA_LIBRARY)
