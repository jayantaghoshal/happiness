# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ipcbd

LOCAL_INIT_RC := ipcbd-infotainment.rc \
                 ipcbd-iplm.rc \
                 ipcbd-dim.rc

LOCAL_MODULE_RELATIVE_PATH := hw

LOCAL_SRC_FILES := \
    src/main.cpp \
    src/service_manager.cpp \
    src/diagnostics_client.cpp

LOCAL_SHARED_LIBRARIES += \
    liblog \
    liblocalconfig \
    libtarmac \
    libutils \
    libhidlbase \
    libhidltransport \
    libhardware \
    libhwbinder \
    libipcommandbus \
    vendor.volvocars.hardware.common@1.0 \
    vendor.volvocars.hardware.vehiclecom@1.0

include $(VENDOR_VOLVOCARS_NATIVE_MODULE_SETTINGS)
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))