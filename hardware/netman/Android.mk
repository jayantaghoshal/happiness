LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := netman
LOCAL_SRC_FILES := src/main.c
LOCAL_INIT_RC := netman.rc
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_EXECUTABLE)



