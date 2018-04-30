# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := VehicleFunctions
LOCAL_MANIFEST_FILE := app/src/main/AndroidManifest.xml
LOCAL_USE_AAPT2 := true
LOCAL_AAPT_FLAGS := --auto-add-overlay
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_TAGS := optional
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_ENABLED := disabled

# Vehicle functions sources
LOCAL_SRC_FILES := $(call all-java-files-under, app/src/main/java)

# Vehicle functions Resources
LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/app/src/main/res

# uxlibrary Relative Path
UX_LIBRARY_PATH := ../../hmi/ComponentLibrary/uxlibrary

# uxlibrary Sources
LOCAL_SRC_FILES += $(call all-java-files-under, $(UX_LIBRARY_PATH)/src/main/java)

# uxlibrary Resources
LOCAL_RESOURCE_DIR += $(LOCAL_PATH)/$(UX_LIBRARY_PATH)/src/main/res

LOCAL_AAPT_FLAGS += --extra-packages com.volvocars.hmi.uxlibrary


LOCAL_STATIC_ANDROID_LIBRARIES := \
    android-support-design \
    android-support-transition \
    android-support-v7-appcompat \
    android-support-v7-recyclerview \
    android-support-v7-cardview \
    android-support-v4

# Include Car library
LOCAL_JAVA_LIBRARIES += android.car
LOCAL_STATIC_JAVA_LIBRARIES += \
        vendor.volvocars.hardware.vehiclehal-V1.0-java \
        volvo.vendorextension \
        carconfigapi

# Android architecture components
LOCAL_STATIC_JAVA_LIBRARIES += \
    vf-arch.core.common \
    vf-arch.lifecycle.common
LOCAL_STATIC_JAVA_AAR_LIBRARIES := \
    vf-arch.lifecycle.extensions \
    vf-arch.lifecycle.livedata \
    vf-arch.lifecycle.livedata-core \
    vf-arch.lifecycle.viewmodel \
    vf-arch.lifecycle.runtime\
    vf-arch.core.runtime


include $(BUILD_PACKAGE)

###########################

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.lifecycle.extensions:libs/archlifecycle/extensions-1.1.0.aar
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.lifecycle.livedata:libs/archlifecycle/livedata-1.1.0.aar
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.lifecycle.livedata-core:libs/archlifecycle/livedata-core-1.1.0.aar
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.lifecycle.viewmodel:libs/archlifecycle/viewmodel-1.1.0.aar
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.lifecycle.runtime:libs/archlifecycle/runtime-1.1.0.aar
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.lifecycle.common:libs/archlifecycle/common-1.1.0.jar

LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.core.runtime:libs/archcore/runtime-1.1.0.aar
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES += vf-arch.core.common:libs/archcore/common-1.1.0.jar
include $(BUILD_MULTI_PREBUILT)
