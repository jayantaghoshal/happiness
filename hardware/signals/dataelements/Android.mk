
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= libdataelements
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

intermediates:= $(local-generated-sources-dir)
GEN := $(intermediates)/Mp_Router_crc.h
crc_gen_tool := $(LOCAL_PATH)/crc16cmd.py
$(GEN): PRIVATE_INPUT_FILE := $(LOCAL_PATH)/vector/gen/MpRouter_Signals.h
$(GEN): PRIVATE_CUSTOM_TOOL = python $(crc_gen_tool) $(PRIVATE_INPUT_FILE) MP_ROUTER_CRC > $@
$(GEN): $(LOCAL_PATH)/vector/gen/MpRouter_Signals.h $(TOOL)
	    $(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

LOCAL_SRC_FILES:= \
    src/dataelemvalue.cpp \
    src/dataelementframework.cpp \
    src/internalsignals.cpp \
    src/vipcomm/VipFramework.cpp \
    src/vipcomm/VipFramework_crc.cpp \
    src/dataelementcommbus_dbus.cpp \
    src/dataelementcommbus_dbus_common.cpp \
    src/dataelementcommbus_dbus_receiver.cpp \
    generated/gen_dataelements.cpp \
    generated/gen_jsonencdec.cpp
    
LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_CPPFLAGS += -frtti # Dataelements use dynamic_cast which requires rtti
LOCAL_SHARED_LIBRARIES +:= liblog libcutils

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/generated \
    $(LOCAL_PATH)/vector

LOCAL_EXPORT_C_INCLUDE_DIRS := \
	$(LOCAL_PATH)/include \
    $(LOCAL_PATH)/generated

include $(BUILD_SHARED_LIBRARY)
