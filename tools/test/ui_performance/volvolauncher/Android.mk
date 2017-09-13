LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := GeneralPerformanceTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/tools/test/ui_performance/volvolauncher
include test/vts/tools/build/Android.host_config.mk

