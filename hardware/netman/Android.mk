LOCAL_PATH := $(call my-dir)

MY_LOCAL_C_INCLUDES=$(LOCAL_PATH)/include
MY_LOCAL_EXPORT_C_INCLUDE_DIRS=$(MY_LOCAL_CONFIG_C_INCLUDES)
MY_LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

include $(CLEAR_VARS)
LOCAL_MODULE := netman
LOCAL_SRC_FILES := src/main.cpp src/netman.cpp
LOCAL_CPPFLAGS := $(MY_LOCAL_CPPFLAGS)
LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_EXPORT_C_INCLUDE_DIRS)
LOCAL_INIT_RC := netman.rc
LOCAL_SHARED_LIBRARIES += liblog liblocalconfig

include $(BUILD_EXECUTABLE)



