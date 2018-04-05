# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsDAISettingComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/vehicle/test/dai_setting
include test/vts/tools/build/Android.host_config.mk