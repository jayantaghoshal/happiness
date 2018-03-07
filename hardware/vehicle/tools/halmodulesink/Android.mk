# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project


LOCAL_PATH:= $(call my-dir)

# Build prebuilt android.volvocars.vendorextension
# ---------------------------------------------

include $(CLEAR_VARS)

LOCAL_MODULE := volvo.vendorextension
LOCAL_MODULE_CLASS := JAVA_LIBRARIES
LOCAL_SRC_FILES := $(call all-java-files-under,  lib/com/volvocars/vendorextension)

# This library needs to be added after the car.mk include.
LOCAL_STATIC_JAVA_LIBRARIES += car-systemtest

LOCAL_STATIC_JAVA_LIBRARIES += \
        android.hardware.automotive.vehicle-V2.0-java \
        vendor.volvocars.hardware.vehiclehal-V1.0-java \
        car-systemtest

# Include car support library, if not already included
ifeq (,$(findstring android.support.car, $(LOCAL_STATIC_JAVA_LIBRARIES)))
LOCAL_STATIC_JAVA_LIBRARIES += android.support.car
endif

LOCAL_JAVA_LIBRARIES += android.car

LOCAL_CERTIFICATE := platform


include $(BUILD_STATIC_JAVA_LIBRARY)


# Create HalModuleSink test app
# ---------------------------------------------
include $(CLEAR_VARS)

LOCAL_SRC_FILES += $(call all-java-files-under, src)
LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res

LOCAL_PACKAGE_NAME := HalModulesSink
LOCAL_MANIFEST_FILE := AndroidManifest.xml
LOCAL_AAPT_FLAGS := --auto-add-overlay
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_USE_AAPT2 := true

LOCAL_PROGUARD_ENABLED := disabled

# Include Car library
LOCAL_JAVA_LIBRARIES += android.car
LOCAL_STATIC_JAVA_LIBRARIES += \
        android.hidl.base-V1.0-java \
        android.hardware.automotive.vehicle-V2.0-java \
        vendor.volvocars.hardware.vehiclehal-V1.0-java \
        services.core \
        android.hidl.manager-V1.0-java \
        vehicle-hal-support-lib \
        car-systemtest \
        volvo.vendorextension

# Android Support library
LOCAL_STATIC_ANDROID_LIBRARIES := \
        android-support-design \
        android-support-transition \
        android-support-v7-appcompat \
        android-support-v7-cardview \
        android-support-v7-recyclerview \
        android-support-v4

# Include support-v7-appcompat, if not already included
ifeq (,$(findstring android-support-v7-appcompat,$(LOCAL_STATIC_JAVA_LIBRARIES)))
LOCAL_RESOURCE_DIR += frameworks/support/v7/appcompat/res
LOCAL_AAPT_FLAGS += --extra-packages android.support.v7.appcompat
LOCAL_STATIC_JAVA_LIBRARIES += android-support-v7-appcompat
endif

include packages/services/Car/car-support-lib/car-support.mk

include $(BUILD_PACKAGE)

