# OBS!!!! LOCAL_PATH MUST be set here !!!
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libasn1
LOCAL_SRC_FILES := generated/infotainmentIpBus.c asn_base/asn_base.c src/asn_print.cpp
LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_CFLAGS := -Wno-unused-parameter -Wno-macro-redefined
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY) 
