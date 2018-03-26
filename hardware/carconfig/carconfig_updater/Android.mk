# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/carconfig_updater_main.cpp \
	src/carconfig_updater.cpp \
	src/vipcom_client.cpp

LOCAL_CFLAGS += -Wno-unused-parameter
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/src \

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -Wno-macro-redefined

LOCAL_MODULE := carconfig-updater


LOCAL_VENDOR_MODULE := true
LOCAL_MULTILIB := 64
LOCAL_INIT_RC := carconfig-updater.rc

LOCAL_SHARED_LIBRARIES := \
	libdataelements \
	liblog \
	libcarconfig_reader \
	libcarconfig_writer \
	libbase \
	libdesipservice \
	libdesipclient \
	libdesip \
	libhidlbase \
	libhidltransport \
	libutils \
	libbinder \
	libhisip \
	libhwbinder

LOCAL_STATIC_LIBRARIES += \
	libccparameterlistgen

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
