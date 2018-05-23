# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := RemoteMediaCtrlGateway

LOCAL_SRC_FILES := $(call all-java-files-under,app/src/main)
LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_STATIC_JAVA_LIBRARIES += \
    vendor.volvocars.hardware.remotectrl-V1.0-java \
    com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService \
    remote-climate-support-lib \
    vehicle-hal-support-lib  \
    android.car \

LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))