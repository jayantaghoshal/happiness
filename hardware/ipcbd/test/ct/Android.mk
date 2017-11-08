LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := localconfig_ipcbd_test_ct
LOCAL_SRC_FILES := localconfig.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MULTILIB := 64
LOCAL_COMPATIBILITY_SUITE := vts
include $(BUILD_PREBUILT)