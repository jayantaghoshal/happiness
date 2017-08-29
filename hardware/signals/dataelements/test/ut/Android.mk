LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := VtsDEUnitTest

LOCAL_MODULE_CLASS := NATIVE_TESTS

intermediates:= $(local-generated-sources-dir)

#
# Generate Mp_Router_crc.h
#
GEN := $(intermediates)/Mp_Router_crc.h
crc_gen_tool := $(LOCAL_PATH)/../../crc16cmd.py
$(GEN): PRIVATE_INPUT_FILE := $(LOCAL_PATH)/../../vector/gen/MpRouter_Signals.h
$(GEN): PRIVATE_CUSTOM_TOOL = python $(crc_gen_tool) $(PRIVATE_INPUT_FILE) MP_ROUTER_CRC > $@
$(GEN): $(LOCAL_PATH)/../../vector/gen/MpRouter_Signals.h $(TOOL)
	    $(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)



LOCAL_SRC_FILES := unittest.cpp \
                   ../../generated/gen_jsonencdec.cpp

LOCAL_C_INCLUDES := ../../include \
                    ../../generated

LOCAL_STATIC_LIBRARIES += libdataelements_unittest 

LOCAL_SHARED_LIBRARIES :=   liblog \
                            libcutils \
                            libbase \
                            libhidlbase \
                            libhidltransport \
                            liblog \
                            libutils \
                            libhwbinder

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



