# OBS!!!! LOCAL_PATH MUST be set here !!!
LOCAL_PATH := $(call my-dir)

#
# Static library. Used in shared lib and for unit tests.
#
include $(CLEAR_VARS)
LOCAL_MODULE := libtarmac_static
LOCAL_SRC_FILES := eventloop/src/Dispatcher.cpp
LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_C_INCLUDES := $(LOCAL_PATH)/eventloop/include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/eventloop/include
LOCAL_MULTILIB := 64
include $(BUILD_STATIC_LIBRARY)

#
# Shared library for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := libtarmac
LOCAL_WHOLE_STATIC_LIBRARIES := libtarmac_static
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/eventloop/include
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MULTILIB := 64
include $(BUILD_SHARED_LIBRARY)

