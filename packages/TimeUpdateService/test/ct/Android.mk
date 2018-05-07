# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsSystemClockTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/packages/TimeUpdateService/test/ct
include test/vts/tools/build/Android.host_config.mk

include $(call all-makefiles-under,$(LOCAL_PATH))

