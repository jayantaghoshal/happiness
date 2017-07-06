# OBS!!!! LOCAL_PATH MUST be set here !!!
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libipcommandbus
LOCAL_SRC_FILES := src/VccIpCmdApi.cpp \
    src/Message.cpp \
    src/MessageDispatcher.cpp \
    src/Pdu.cpp \
    src/socket.cpp \
    src/UdpSocket.cpp \
    src/TcpSocket.cpp \
    src/vcc_pdu_header.cpp \
    src/net_serializer.cpp \
    src/net_deserializer.cpp \
    src/TimeoutInfo.cpp \
    src/TransportServices.cpp \
    src/local_config_parameters.cpp
#    src/shutdown_client.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined -DLOG_NDEBUG=0 -g
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := libtarmac liblocalconfig
LOCAL_STATIC_LIBRARIES := libasn1

include $(BUILD_STATIC_LIBRARY) 
