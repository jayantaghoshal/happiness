# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
            $(call all-subdir-java-files)

LOCAL_MODULE_TAGS := optional

#JNI Shared
LOCAL_JNI_SHARED_LIBRARIES := liblocalconfig_library_jni

# This is the target being built.
LOCAL_MODULE:= com.volvocars.localconfig_java_jni_lib

LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_STATIC_JAVA_LIBRARY)
