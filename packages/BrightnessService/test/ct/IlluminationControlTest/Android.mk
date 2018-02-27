# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)
# Build java application for test

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
ANDROID_PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)

LOCAL_STATIC_JAVA_LIBRARIES := ctstestrunner \
                               junit \
                               android-support-test \

LOCAL_JAVA_LIBRARIES := android.test.runner
LOCAL_PACKAGE_NAME := TestIlluminationControlJava

LOCAL_COMPATIBILITY_SUITE := cts
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_REQUIRED_MODULES := InjectFlexRayIllumination

LOCAL_MULTILIB := 64

include $(BUILD_CTS_PACKAGE)

include $(CLEAR_VARS)
LOCAL_MODULE := InjectFlexRayIllumination
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := InjectFlexRayIlluminationTest/main.cpp

LOCAL_SHARED_LIBRARIES += \
    liblog \
    libdataelements \

LOCAL_MULTILIB := 64
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -Wno-macro-redefined

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
ANDROID_PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))