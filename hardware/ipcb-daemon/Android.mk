LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := infotainment-ip-service
LOCAL_INIT_RC := infotainment-ip-service.rc
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true

# infotainment-ip-service
MY_ROOT := infotainment_ip_service/
LOCAL_SRC_FILES := $(MY_ROOT)src/main.cpp \
    $(MY_ROOT)src/util/type_conversion_helpers.cpp \
    $(MY_ROOT)src/util/local_config.cpp \
    $(MY_ROOT)src/services/iplm_service.cpp \
    $(MY_ROOT)src/services/gnss_service.cpp \
    $(MY_ROOT)src/services/diagnostics_client.cpp \
    $(MY_ROOT)src/IpService.cpp \
    $(MY_ROOT)src/service_manager.cpp \
    $(MY_ROOT)src/Connectivity.cpp


# libasn1 - not built as static lib due to dependence problems in Android .mk files
# There are a few warnings in these .c files that can be supressed by:
# LOCAL_CFLAGS += -Wno-unused-parameter
# this will however disable these warnings for all .c and .cpp file which is not what we want.
# How can we disable these warnings only for these .c files??
LOCAL_SRC_FILES += libasn1/generated/infotainmentIpBus.c \
    libasn1/asn_base/asn_base.c \
    libasn1/src/asn_print.cpp


# libipcommandbus - not built as static lib due to dependence problems in Android .mk files
LOCAL_SRC_FILES += libipcommandbus/src/VccIpCmdApi.cpp \
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

# Gnss
LOCAL_SRC_FILES += \
    infotainment_ip_service/src/binderimpl/gnss/AGnssCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/AGnss.cpp \
    infotainment_ip_service/src/binderimpl/gnss/AGnssRilCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/AGnssRil.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssBatchingCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssBatching.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssConfiguration.cpp \
    infotainment_ip_service/src/binderimpl/gnss/Gnss.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssDebug.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssGeofenceCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssGeofencing.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssMeasurementCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssMeasurement.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssNavigationMessageCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssNavigationMessage.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssNiCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssNi.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssXtraCallback.cpp \
    infotainment_ip_service/src/binderimpl/gnss/GnssXtra.cpp


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
    android.hardware.gnss@1.0 \

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/infotainment_ip_service/src \
    $(LOCAL_PATH)/libasn1/include \
    $(LOCAL_PATH)/libipcommandbus/include \
    $(LOCAL_PATH)/infotainment_ip_service/src/binderimpl/gnss

LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)