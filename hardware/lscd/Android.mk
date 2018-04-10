# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lscd

LOCAL_INIT_RC := lscd.rc


LOCAL_SRC_FILES := \
    src/main.cpp \
    src/lscservice.cpp

LOCAL_C_INCLUDES := src/

LOCAL_SHARED_LIBRARIES += \
    liblog \
    liblocalconfig \
    libtarmac \
    libdataelements \
    libutils \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    vendor.volvocars.hardware.common@1.0 \
    vendor.volvocars.hardware.vehiclecom@1.0 \
    vendor.volvocars.hardware.iplm@1.0

include $(VENDOR_VOLVOCARS_NATIVE_MODULE_SETTINGS)
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))



