LOCAL_PATH := $(call my-dir)

MY_LOCAL_C_INCLUDES=$(LOCAL_PATH)/tiny-ipcommandbus/include/
MY_LOCAL_EXPORT_C_INCLUDE_DIRS=$(MY_LOCAL_CONFIG_C_INCLUDES)
MY_LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

include $(CLEAR_VARS)
LOCAL_MODULE := packet_injector

# LOCAL_INIT_RC := iplmd.rc

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

LOCAL_SRC_FILES := \
    tiny-ipcommandbus/src/udp_packet_injector.cpp \
    tiny-ipcommandbus/src/main.cpp \
    tiny-ipcommandbus/src/IpCmdTypes.cpp \
    tiny-ipcommandbus/src/Pdu.cpp \
    tiny-ipcommandbus/src/vcc_pdu_header.cpp \
    tiny-ipcommandbus/src/net_serializer.cpp \
    tiny-ipcommandbus/src/net_deserializer.cpp \


LOCAL_CPPFLAGS := $(MY_LOCAL_CPPFLAGS)
LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_EXPORT_C_INCLUDE_DIRS)

LOCAL_SHARED_LIBRARIES += \
    libtarmac \
    liblog \
    libutils \
    libhidlbase \
    libhidltransport \
    libhwbinder


LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)



