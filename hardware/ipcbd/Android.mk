LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := ipcbd

LOCAL_INIT_RC := ipcbd-infotainment.rc \
                 ipcbd-iplm.rc

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true

# ipcb-daemon

LOCAL_SRC_FILES := \
    src/main.cpp \
    src/service_manager.cpp \
    src/diagnostics_client.cpp

# There are a few warnings in these .c files that can be supressed by:
# LOCAL_CFLAGS += -Wno-unused-parameter
# this will however disable these warnings for all .c and .cpp file which is not what we want.
# How can we disable these warnings only for these .c files??

# libipcommandbus - not built as static lib due to dependence problems in Android .mk files
LOCAL_SRC_FILES += \
    libipcommandbus/src/IpCmdTypes.cpp \
    libipcommandbus/src/Message.cpp \
    libipcommandbus/src/MessageDispatcher.cpp \
    libipcommandbus/src/Pdu.cpp \
    libipcommandbus/src/socket.cpp \
    libipcommandbus/src/UdpSocket.cpp \
    libipcommandbus/src/TcpSocket.cpp \
    libipcommandbus/src/vcc_pdu_header.cpp \
    libipcommandbus/src/net_serializer.cpp \
    libipcommandbus/src/net_deserializer.cpp \
    libipcommandbus/src/TimeoutInfo.cpp \
    libipcommandbus/src/TransportServices.cpp \
    libipcommandbus/src/local_config_parameters.cpp
#    libipcommandbus/src/shutdown_client.cpp

# TODO check if libhwbinder is the correct one to use here
#LOCAL_CPPFLAGS := -fexceptions -frtti -DLOG_NDEBUG=0 -g
LOCAL_CPPFLAGS := -fexceptions -DLOG_NDEBUG=0 -g

LOCAL_SHARED_LIBRARIES += \
    liblog \
    liblocalconfig \
    libtarmac \
    libutils \
    libhidlbase \
    libhidltransport \
    libhardware \
    libhwbinder \
    vendor.volvocars.hardware.common@1.0 \
    vendor.volvocars.hardware.vehiclecom@1.0

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/ipcbd/src \
    $(LOCAL_PATH)/libipcommandbus/include

LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
