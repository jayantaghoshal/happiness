LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#Build python test
LOCAL_MODULE := VtsDaemonTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/tools/test/daemon_test
include test/vts/tools/build/Android.host_config.mk
