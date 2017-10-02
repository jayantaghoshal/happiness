LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsNetmandIptablesComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/netman/test/ct/iptables
include test/vts/tools/build/Android.host_config.mk
