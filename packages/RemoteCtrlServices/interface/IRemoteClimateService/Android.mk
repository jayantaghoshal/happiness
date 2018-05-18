# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-Iaidl-files-under, src)
LOCAL_SRC_FILES += $(call all-java-files-under, src)
LOCAL_JAVA_LIBRARIES += \
    android.car \
    vehicle-hal-support-lib \
    vendor.volvocars.hardware.remotectrl-V1.0-java \

include $(BUILD_STATIC_JAVA_LIBRARY)
