LOCAL_PATH:= $(call my-dir)
LOCAL_CERTIFICATE := platform

include $(CLEAR_VARS)
LOCAL_MODULE := carconfigapi
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under, *)

include $(BUILD_STATIC_JAVA_LIBRARY)


