LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsNetmandComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/netman/test/ct
include test/vts/tools/build/Android.host_config.mk
