LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := vendor.volvocars.cloudservice.IfsApi
LOCAL_SRC_FILES := $(call all-Iaidl-files-under, src)
include $(BUILD_JAVA_LIBRARY)
