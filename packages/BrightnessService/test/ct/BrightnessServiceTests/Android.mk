# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

# Build java application for test
include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

LOCAL_STATIC_JAVA_LIBRARIES := ctstestrunner \
                               junit \
                               android-support-test \

LOCAL_JAVA_LIBRARIES := android.test.runner
LOCAL_PACKAGE_NAME := TestIlluminationControlJava

LOCAL_COMPATIBILITY_SUITE := cts
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true

include $(BUILD_CTS_PACKAGE)

# Build cpp part o f test
include $(CLEAR_VARS)
LOCAL_MODULE := InjectDEForBrightness
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := InjectDE/main.cpp

LOCAL_SHARED_LIBRARIES +=  liblog \
                           libdataelements \

LOCAL_MULTILIB := 64
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -Wno-macro-redefined

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)

include $(BUILD_EXECUTABLE)
include $(call all-makefiles-under,$(LOCAL_PATH))
