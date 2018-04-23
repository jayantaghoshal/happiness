# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := RemoteClimateService
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_AAPT_FLAGS += --extra-packages com.android.car --auto-add-overlay
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_STATIC_JAVA_LIBRARIES += \
	vehicle-hal-support-lib \
	android.hardware.automotive.vehicle-V2.0-java \
	vendor.volvocars.hardware.remotectrl-V1.0-java \
	com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService \
	volvo.vendorextension \
	vendor.volvocars.hardware.vehiclehal-V1.0-java \
	android.car \
	remote-climate-support-lib

LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)

#include $(CLEAR_VARS)

#LOCAL_SRC_FILES := \
#    $(call all-java-files-under, src) \
#    $(call all-java-files-under, test)

#LOCAL_PACKAGE_NAME := RemoteClimateServiceUnitTests

#LOCAL_MODULE_TAGS := optional

#LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_APPS)
#PRODUCT_OUT:= $(TARGET_OUT_DATA_APPS)

#LOCAL_INSTRUMENTATION_FOR := RemoteClimateService

#LOCAL_STATIC_JAVA_LIBRARIES += \
#	junit-host \
#	mockito-host \
#	android-support-test \

#LOCAL_JAVA_LIBRARIES += \
#	android.test.runner
#    android-support-test \
#

#LOCAL_PROGUARD_FLAG_FILES := proguard.flags
#LOCAL_PROGUARD_ENABLED := disabled
#LOCAL_MULTILIB := 64

#include $(BUILD_HOST_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))
