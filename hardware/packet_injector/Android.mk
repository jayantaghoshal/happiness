LOCAL_PATH := $(call my-dir)


#
# Static library. Used in shared lib and for unit tests.
#
include $(CLEAR_VARS)
LOCAL_MODULE := libpacketinjector_static

LOCAL_SRC_FILES := src/udp_packet_injector.cpp

LOCAL_SHARED_LIBRARIES := \
    libtarmac \
    liblog \
    libipcommandbus \
    libutils \
    libhidlbase \
    libhidltransport \
    libhwbinder

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(LOCAL_PATH)/include

LOCAL_MULTILIB := 64
include $(BUILD_STATIC_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))