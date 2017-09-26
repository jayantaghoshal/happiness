LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsNetmandNamespaceComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/netman/test/ct/namespace_configuration
include test/vts/tools/build/Android.host_config.mk
