# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := remote-climate-support-lib
LOCAL_SRC_FILES := $(call all-java-files-under,app/src/main)

LOCAL_JAVA_LIBRARIES += \
    vehicle-hal-support-lib  \
    android.hardware.automotive.vehicle-V2.0-java \
    android.car \
    vendor.volvocars.hardware.remotectrl-V1.0-java \
    com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService \

LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml
#LOCAL_RESOURCE_DIR += \
#    $(LOCAL_PATH)/app/src/main/res

include $(BUILD_STATIC_JAVA_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
