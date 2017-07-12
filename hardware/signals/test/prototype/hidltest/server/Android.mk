
LOCAL_PATH := $(call my-dir)


###############################################################################
# HIDL service
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := hidl-test-service


LOCAL_SRC_FILES := \
    hidl-test-server.cpp \
    SignalsImpl.cpp \
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
    libhwbinder \
    libcutils

LOCAL_CFLAGS += -Wall -Wextra -Werror

include $(BUILD_EXECUTABLE)
