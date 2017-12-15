LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_MODULE:= liblocalconfig_library_jni

LOCAL_CPPFLAGS := -fexceptions -DLOG_NDEBUG=0 -g

# All of the source files that we will compile.
LOCAL_SRC_FILES:= \
    local_config_lib_jni.cpp

# All of the shared libraries we link against.
LOCAL_SHARED_LIBRARIES := \
    libnativehelper \
    libcutils \
    libutils \
    liblog \
    liblocalconfig

# No static libraries.
LOCAL_STATIC_LIBRARIES :=

# Also need the JNI headers.
LOCAL_C_INCLUDES += \
    $(JNI_H_INCLUDE)

# No specia compiler flags.
LOCAL_CFLAGS +=

LOCAL_MULTILIB := 64

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))