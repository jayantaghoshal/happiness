# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vsd_unittest

LOCAL_SRC_FILES :=  vsd_unit_test.cpp \
                    mocks/vsm_inject_mock.cpp \
                    mocks/vsm_sink_mock.cpp \
                    mocks/misc_mock.cpp \
                    ../../src/message_inject.cpp \
                    ../../src/vsm.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/stubs \
                    $(LOCAL_PATH)/mocks \
                    $(LOCAL_PATH)/../../src


LOCAL_SHARED_LIBRARIES :=   liblog \
                            libcutils \
                            libdataelements

LOCAL_STATIC_LIBRARIES :=   libgmock

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

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)



