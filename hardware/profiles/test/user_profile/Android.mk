# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsProfileComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/profiles/test/user_profile
include test/vts/tools/build/Android.host_config.mk

