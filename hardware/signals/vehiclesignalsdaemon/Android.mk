LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := vehicle-signals-daemon
LOCAL_INIT_RC := vehicle-signals-daemon.rc
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true

LOCAL_SRC_FILES := src/main.cpp

LOCAL_CPPFLAGS := -fexceptions -DLOG_NDEBUG=0
LOCAL_SHARED_LIBRARIES += \
    liblog
    
include $(BUILD_EXECUTABLE)
