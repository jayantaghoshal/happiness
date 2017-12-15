LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := android.volvocars.localconfig
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under, java) $(call all-Iaidl-files-under, java)
include $(BUILD_JAVA_LIBRARY)
