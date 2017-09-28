LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VtsNetmandKernelIpStackComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/netman/test/ct/kernel_ip_stack
include test/vts/tools/build/Android.host_config.mk
