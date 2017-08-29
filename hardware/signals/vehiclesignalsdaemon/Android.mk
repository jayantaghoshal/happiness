LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= vehicle-signals-daemon
LOCAL_INIT_RC := vehicle-signals-daemon.rc
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true

LOCAL_SRC_FILES:= src/main.cpp \
    src/vfc_handler.cpp \
    src/signallog.cpp \
    src/desip_handler.cpp \
    src/vsm_inject.cpp \
    src/vsm.cpp \
    src/message_inject.cpp \
    src/vsm_sink.cpp

LOCAL_SHARED_LIBRARIES += libc++ liblog libdataelements 

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined


LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/src 
  
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

include $(BUILD_EXECUTABLE)
