LOCAL_PATH:= $(call my-dir)

localconfig_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
localconfig_STATIC_LIBRARIES := libjsoncpp liblog
localconfig_SRC_FILES := \
    src/localconfig.cpp \
    src/local_config_reader_interface.cpp \
    src/local_config_reader.cpp

localconfig_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

#
# Static library. Used in shared lib and for unit tests.
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig_static
LOCAL_SRC_FILES := $(localconfig_SRC_FILES)
LOCAL_CPPFLAGS := $(localconfig_CPPFLAGS)
LOCAL_C_INCLUDES := $(localconfig_EXPORT_C_INCLUDE_DIRS)
LOCAL_STATIC_LIBRARIES := $(localconfig_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(localconfig_EXPORT_C_INCLUDE_DIRS)
LOCAL_MULTILIB := 64
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig_static
LOCAL_SRC_FILES := $(localconfig_SRC_FILES)
LOCAL_CPPFLAGS := $(localconfig_CPPFLAGS)
LOCAL_C_INCLUDES := $(localconfig_EXPORT_C_INCLUDE_DIRS)
LOCAL_STATIC_LIBRARIES := $(localconfig_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(localconfig_EXPORT_C_INCLUDE_DIRS)
LOCAL_MULTILIB := 64
include $(BUILD_HOST_STATIC_LIBRARY)

#
# Shared library for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig
LOCAL_VENDOR_MODULE := true
LOCAL_WHOLE_STATIC_LIBRARIES := liblocalconfig_static
LOCAL_STATIC_LIBRARIES := $(localconfig_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(localconfig_EXPORT_C_INCLUDE_DIRS)
LOCAL_MULTILIB := 64
include $(BUILD_SHARED_LIBRARY)

#
# Copy the default JSON file to /vendor/etc/config
#
include $(CLEAR_VARS)
LOCAL_MODULE := localconfig.json
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_RELATIVE_PATH := config
LOCAL_SRC_FILES := misc/localconfig.json

# We need an init.rc which will initialize with a default localconfig.json if needed.
LOCAL_INIT_RC := misc/localconfig.rc

# This is probably not needed, please review...
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)
include $(call all-makefiles-under,$(LOCAL_PATH))