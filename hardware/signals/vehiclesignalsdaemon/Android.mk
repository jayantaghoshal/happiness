# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := src/main.cpp \
        src/vfc_handler.cpp \
        src/signallog.cpp \
        src/desip_handler.cpp \
        src/vsm_inject.cpp \
        src/vsm.cpp \
        src/message_inject.cpp \
        src/vsm_sink.cpp

LOCAL_CFLAGS += -Wno-unused-parameter
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -Wno-macro-redefined

LOCAL_MODULE := vehicle-signals-daemon

LOCAL_VENDOR_MODULE := true
LOCAL_MULTILIB := 64
LOCAL_INIT_RC := vehicle-signals-daemon.rc

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libc++ \
    liblog \
    libdataelements \
    libhidlbase \
    libhidltransport \
    libdesip

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
