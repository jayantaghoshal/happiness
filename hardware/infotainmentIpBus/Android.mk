# OBS!!!! LOCAL_PATH MUST be set here !!!
LOCAL_PATH := $(call my-dir)

#
# Static library. Used in shared lib and for unit tests.
#
include $(CLEAR_VARS)
LOCAL_MODULE := libinfotainmentbusasn_static
LOCAL_SRC_FILES += \
    generated/infotainmentIpBus.c \
    generated/VccIpCmdApi.cpp \
    asn_base/asn_base.c \
    src/asn_print.cpp \
    src/type_conversion_helpers.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_MULTILIB := 64
include $(BUILD_STATIC_LIBRARY)

#
# Shared library for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := libinfotainmentbusasn
LOCAL_WHOLE_STATIC_LIBRARIES := libinfotainmentbusasn_static
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MULTILIB := 64

include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))