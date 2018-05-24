# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Build the VendorVCC priv app.
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := RemoteCtrlGeneralSettingsGateway
LOCAL_SRC_FILES := $(call all-java-files-under,app/src/main)
LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_MODULE_TAGS := optional
LOCAL_MULTILIB := 64
LOCAL_STATIC_JAVA_LIBRARIES += \
        vendor.volvocars.hardware.remotectrl-V1.0-java \
        vehicle-hal-support-lib  \
        android.hardware.automotive.vehicle-V2.0-java \
        android.car \
        remote-climate-support-lib\
        com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))
