# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)


#Build changecarconfig c++ binary
include $(CLEAR_VARS)
LOCAL_MODULE := changecarconfig

LOCAL_SRC_FILES := changecarconfig.cpp

LOCAL_SHARED_LIBRARIES := libcarconfig_reader \
					      libcarconfig_writer

# We only build for 64 bit.
LOCAL_MULTILIB := 64

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

include $(BUILD_EXECUTABLE)


#Build readcarconfig c++ binary
include $(CLEAR_VARS)
LOCAL_MODULE := readcarconfig

LOCAL_SRC_FILES := readcarconfig.cpp

LOCAL_SHARED_LIBRARIES := libcarconfig_reader

# We only build for 64 bit.
LOCAL_MULTILIB := 64

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

include $(BUILD_EXECUTABLE)