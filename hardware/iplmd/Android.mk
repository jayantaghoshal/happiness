LOCAL_PATH := $(call my-dir)

MY_LOCAL_C_INCLUDES=$(LOCAL_PATH)/include
MY_LOCAL_EXPORT_C_INCLUDE_DIRS=$(MY_LOCAL_CONFIG_C_INCLUDES)
MY_LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

include $(CLEAR_VARS)
LOCAL_MODULE := iplmd

LOCAL_INIT_RC := iplmd.rc

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

LOCAL_SRC_FILES := \
    src/main.cpp \
    src/iplmService.cpp \
    src/local_config.cpp
LOCAL_CPPFLAGS := $(MY_LOCAL_CPPFLAGS)
LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_EXPORT_C_INCLUDE_DIRS)

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
    vendor.volvocars.hardware.ipcb@1.0

LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)



