# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := com.volvocars.settingsstorageservice.ISettingsStorageManager
LOCAL_SRC_FILES := $(call all-java-files-under, src) \
src/com/volvocars/settingsstorageservice/ISettingsStorageManager.aidl \
src/com/volvocars/settingsstorageservice/ISettingsStorageManagerCallback.aidl \

include $(BUILD_STATIC_JAVA_LIBRARY)
