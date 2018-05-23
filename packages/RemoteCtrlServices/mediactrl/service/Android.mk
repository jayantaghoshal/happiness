# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := RemoteMediaCtrlService
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_SRC_FILES := $(call all-java-files-under,app/src/main)
LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/app/src/main/res
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml


LOCAL_AAPT_FLAGS += \
	--extra-packages exoplayer

LOCAL_STATIC_JAVA_AAR_LIBRARIES +=\
	 exoplayer\
     exoplayer-core

LOCAL_STATIC_JAVA_LIBRARIES += \
	com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService \
	vendor.volvocars.hardware.remotectrl-V1.0-java \
    vehicle-hal-support-lib  \
    android.hardware.automotive.vehicle-V2.0-java \


LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)

#######################
include $(CLEAR_VARS)

LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES +=\
	exoplayer:app/compileonlylibs/exoplayer-2.7.3.aar\
    exoplayer-core:app/compileonlylibs/exoplayer-core-2.7.3.aar

include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)

include $(call all-makefiles-under,$(LOCAL_PATH))
