# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := RemoteAudioCtrlAppService
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_STATIC_JAVA_LIBRARIES := com.volvocars.remotectrlservices.audioctrl.IAudioCtrlService
LOCAL_STATIC_JAVA_LIBRARIES += android.car
LOCAL_MULTILIB := 64


include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))