# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := RemoteClimateGateway
LOCAL_SRC_FILES := $(call all-java-files-under,app/src/main)

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml
LOCAL_RESOURCE_DIR += \
    $(LOCAL_PATH)/app/src/main/res

LOCAL_STATIC_JAVA_LIBRARIES += \
    vendor.volvocars.hardware.remotectrl-V1.0-java \
    com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService \
    vehicle-hal-support-lib  \
    android.hardware.automotive.vehicle-V2.0-java \
    android.car \
    remote-climate-support-lib \

include $(BUILD_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))

