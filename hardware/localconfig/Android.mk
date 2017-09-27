LOCAL_PATH:= $(call my-dir)

MY_LOCALCONFIG_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
MY_LOCALCONFIG_STATIC_LIBRARIES := libjsoncpp

#
# Static library. Used in shared lib and for unit tests.
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig_static
LOCAL_SRC_FILES := src/localconfig.cpp \
                   src/local_config_reader_interface.cpp \
                   src/local_config_reader.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_STATIC_LIBRARIES := $(MY_LOCALCONFIG_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCALCONFIG_EXPORT_C_INCLUDE_DIRS)
# We only build for 64 bit.
LOCAL_MULTILIB := 64
include $(BUILD_STATIC_LIBRARY)

#
# Shared library for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblocalconfig
LOCAL_VENDOR_MODULE := true
LOCAL_WHOLE_STATIC_LIBRARIES := liblocalconfig_static
LOCAL_STATIC_LIBRARIES := $(MY_LOCALCONFIG_STATIC_LIBRARIES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCALCONFIG_EXPORT_C_INCLUDE_DIRS)
# We need an init.rc which will initialize with a default localconfig.json if needed.
LOCAL_INIT_RC := misc/localconfig.rc
LOCAL_REQUIRED_MODULES := localconfig.json
# We only build for 64 bit.
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
# This is probably not needed, please review...
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)
include $(call all-makefiles-under,$(LOCAL_PATH))