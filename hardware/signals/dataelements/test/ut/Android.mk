# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := VtsDEUnitTest

LOCAL_SRC_FILES := unittest.cpp \
                   server_test.cpp

LOCAL_C_INCLUDES := ../../include \
                    ../../generated

LOCAL_STATIC_LIBRARIES += libdataelements_unittest \
                        libgmock

LOCAL_SHARED_LIBRARIES :=   liblog \
                            libcutils \
                            libbase \
                            libhidlbase \
                            libhidltransport \
                            liblog \
                            libutils \
                            libhwbinder \
                            vendor.volvocars.hardware.signals@1.0

LOCAL_CPPFLAGS := -std=c++1z \
                  -g \
                  -Werror \
                  -Wall \
                  -Wextra \
                  -Wunused \
                  -Wno-non-virtual-dtor \
                  -fexceptions \
                  -Wno-macro-redefined \
                  -DUNIT_TEST \
                  -Wno-missing-braces \
                  -Wmissing-field-initializers \
                  -Wunused-variable


LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)



