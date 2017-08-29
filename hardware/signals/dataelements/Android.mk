
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= libdataelements
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

intermediates:= $(local-generated-sources-dir)

#
# Generate Mp_Router_crc.h
#
GEN := $(intermediates)/Mp_Router_crc.h
crc_gen_tool := $(LOCAL_PATH)/crc16cmd.py
$(GEN): PRIVATE_INPUT_FILE := $(LOCAL_PATH)/vector/gen/MpRouter_Signals.h
$(GEN): PRIVATE_CUSTOM_TOOL = python $(crc_gen_tool) $(PRIVATE_INPUT_FILE) MP_ROUTER_CRC > $@
$(GEN): $(LOCAL_PATH)/vector/gen/MpRouter_Signals.h $(TOOL)
	    $(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)


HIDL := $(HOST_OUT_EXECUTABLES)/hidl-gen$(HOST_EXECUTABLE_SUFFIX)
#
# Generate C++ code for HIDL interface ISignals.hal, ISignalsChangedCallback.hal and ISignalsServiceCallback.hal
#
GEN := $(intermediates)/ihu/signals/1.0/SignalsAll.cpp \
        $(intermediates)/ihu/signals/1.0/SignalsChangedCallbackAll.cpp \
        $(intermediates)/ihu/signals/1.0/SignalsServiceCallbackAll.cpp
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/hidl/interface/ihu/signals/1.0/ISignals.hal \
                        $(LOCAL_PATH)/hidl/interface/ihu/signals/1.0/ISignalsChangedCallback.hal \
                        $(LOCAL_PATH)/hidl/interface/ihu/signals/1.0/ISignalsServiceCallback.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
local_path_stored := $(LOCAL_PATH)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Lc++ \
        -randroid.hidl:system/libhidl/transport \
        -rihu:$(local_path_stored)/hidl/interface/ihu ihu.signals@1.0

$(GEN): $(local_path_stored)/hidl/interface/ihu/signals/1.0/ISignals.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)


LOCAL_SRC_FILES:= \
    src/dataelemvalue.cpp \
    src/dataelementframework.cpp \
    src/internalsignals.cpp \
    src/vipcomm/VipFramework.cpp \
    src/vipcomm/VipFramework_crc.cpp \
    generated/gen_dataelements.cpp \
    generated/gen_jsonencdec.cpp \
    src/dataelementcommbus_hidl.cpp
    
LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_SHARED_LIBRARIES :=   liblog \
                            libcutils \
                            libbase \
                            libhidlbase \
                            libhidltransport \
                            liblog \
                            libutils \
                            libhwbinder

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/generated \
    $(LOCAL_PATH)/vector

LOCAL_EXPORT_C_INCLUDE_DIRS := \
	$(LOCAL_PATH)/include \
    $(LOCAL_PATH)/generated \

LOCAL_MULTILIB := 64

include $(BUILD_SHARED_LIBRARY)


#
# Static library. Used for unit tests.
#
include $(CLEAR_VARS)
LOCAL_MODULE := libdataelements_unittest

LOCAL_SRC_FILES:= \
    src/dataelemvalue.cpp \
    src/dataelementframework.cpp \
    src/dataelementcommbus_standalone.cpp \
    src/vipcomm/VipFramework.cpp \
    src/vipcomm/VipFramework_crc.cpp \
    generated/gen_dataelements.cpp \
    generated/gen_jsonencdec.cpp

    
LOCAL_CPPFLAGS := -std=c++1z \
                -g \
                -Wno-non-virtual-dtor \
                -DUNIT_TEST \
                -fexceptions \
                -Wno-non-virtual-dtor \
                -Wno-unused-parameter \
                -Wno-macro-redefined

LOCAL_SHARED_LIBRARIES :=   liblog \
                            libcutils \
                            libbase \
                            libhidlbase \
                            libhidltransport \
                            liblog \
                            libutils \
                            libhwbinder

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/generated \
    $(LOCAL_PATH)/vector

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/generated \
    $(LOCAL_PATH)/vector

# We only build for 64 bit.
LOCAL_MULTILIB := 64
include $(BUILD_STATIC_LIBRARY)
