
LOCAL_PATH := $(call my-dir)


###############################################################################
# HIDL client
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := hidl-test-client

LOCAL_SRC_FILES := \
    hidl-test-client.cpp \
    ../hidl-generated/vcc/vcctest/1.0/SignalsAll.cpp \
    ../hidl-generated/vcc/vcctest/1.0/SignalsCallbackAll.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../hidl-generated

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libhidlbase \
    libhidltransport \
    liblog \
    libprotobuf-cpp-lite \
    libutils \
    libcutils \
    libhwbinder

LOCAL_CFLAGS += -Wall -Wextra -Werror


LOCAL_MULTILIB := 64

include $(BUILD_EXECUTABLE)
