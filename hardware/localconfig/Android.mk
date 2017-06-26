LOCAL_PATH:= $(call my-dir)

MY_LOCALCONFIG_SRC := src/localconfig.cpp
MY_LOCALCONFIG_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
MY_LOCALCONFIG_C_INCLUDES := $(LOCAL_PATH)/include
MY_LOCALCONFIG_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
MY_LOCALCONFIG_STATIC_LIBRARIES += libjsoncpp

#
# Shared library for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig
LOCAL_SRC_FILES := $(MY_LOCALCONFIG_SRC)
LOCAL_CPPFLAGS := $(MY_LOCALCONFIG_CPPFLAGS)
LOCAL_C_INCLUDES := $(MY_LOCALCONFIG_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := $(MY_LOCALCONFIG_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCALCONFIG_EXPORT_C_INCLUDE_DIRS)
include $(BUILD_SHARED_LIBRARY)

#
# Static library for unit test
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig_static
LOCAL_SRC_FILES := $(MY_LOCALCONFIG_SRC)
LOCAL_CPPFLAGS := $(MY_LOCALCONFIG_CPPFLAGS)
LOCAL_C_INCLUDES := $(MY_LOCALCONFIG_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := $(MY_LOCALCONFIG_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCALCONFIG_EXPORT_C_INCLUDE_DIRS)
include $(BUILD_STATIC_LIBRARY)

#
# Copy the default JSON file to /vendor/etc
#
include $(CLEAR_VARS)
LOCAL_MODULE := localconfig.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/../vendor/etc
LOCAL_SRC_FILES := misc/localconfig.json
include $(BUILD_PREBUILT)

#
# copy rc file that copies default json file from
# /vendor/etc/ to /oem_config/
#
include $(CLEAR_VARS)
LOCAL_MODULE := setupfirstfiles.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/../vendor/etc/init
LOCAL_SRC_FILES := misc/setupfirstfiles.rc
include $(BUILD_PREBUILT)
