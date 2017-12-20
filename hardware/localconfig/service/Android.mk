# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, java)

LOCAL_PACKAGE_NAME := LcfService

# Each update should be signed by OEMs
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_STATIC_JAVA_LIBRARIES += android.volvocars.localconfig

LOCAL_STATIC_JAVA_LIBRARIES += com.volvocars.localconfig_java_jni_lib

LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))
