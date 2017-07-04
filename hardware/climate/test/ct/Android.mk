LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsClimateComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/climate/test/ct
include test/vts/tools/build/Android.host_config.mk

