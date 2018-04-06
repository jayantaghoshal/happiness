# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := soundnotifications
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_VENDOR_MODULE := true
LOCAL_PROPRIETARY_MODULE := true
LOCAL_INIT_RC := soundnotifications.rc

LOCAL_SRC_FILES := \
	src/soundnotifications.cpp \
	src/soundwrapper.cpp \
	src/turnindicator.cpp \
	src/parking_brake.cpp \
	src/beltreminder.cpp \

LOCAL_SHARED_LIBRARIES := \
    libdataelements \
    libbase \
    libhidlbase \
    libhidltransport \
    libtarmac \
    libhwbinder \
    liblog \
    libutils \
    libcutils \
    libbinder \
    libtraceclient \
    vendor.delphi.audiomanager@1.0

LOCAL_STATIC_LIBRARIES := \
    libaudiotable

include $(VENDOR_VOLVOCARS_NATIVE_MODULE_SETTINGS_HQ)

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
