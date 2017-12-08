LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := iplmd

LOCAL_INIT_RC := iplmd.rc

LOCAL_MODULE_RELATIVE_PATH := hw

LOCAL_SRC_FILES := \
    src/main.cpp \
    src/iplmService.cpp \
    src/local_config.cpp

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

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



