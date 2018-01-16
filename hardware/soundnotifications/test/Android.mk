# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := soundnotification_test
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_VENDOR_MODULE := true
LOCAL_PROPRIETARY_MODULE := true

LOCAL_SRC_FILES := soundnotification_test.cpp

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    liblog \
    libutils \
    libcutils \
    libbinder \
    libtraceclient \
    libAudioManagerBinder \
    libAudioManagerCallbackBinder

LOCAL_STATIC_LIBRARIES := libaudiotable

LOCAL_CFLAGS  += -Wall -Wextra -Werror
LOCAL_CPPFLAGS := -std=c++14 -Wall -Wextra -Werror -Wno-c++11-narrowing -fexceptions
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
