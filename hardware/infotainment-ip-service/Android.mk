LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := infotainment-ip-service
MY_ROOT := infotainment_ip_service/
LOCAL_SRC_FILES := $(MY_ROOT)src/main.cpp \
    $(MY_ROOT)src/main_loop.cpp \
    $(MY_ROOT)src/util/type_conversion_helpers.cpp \
    $(MY_ROOT)src/util/local_config.cpp \
    $(MY_ROOT)src/services/iplm_service.cpp \
    $(MY_ROOT)src/services/gnss_service.cpp \
    $(MY_ROOT)src/services/diagnostics_client.cpp \
    $(MY_ROOT)src/IpService.cpp \
    $(MY_ROOT)src/service_manager.cpp \
    $(MY_ROOT)src/Connectivity.cpp

LOCAL_CPPFLAGS := -fexceptions -frtti -DLOG_NDEBUG=0 -g
LOCAL_INIT_RC := infotainment-ip-service.rc
LOCAL_STATIC_LIBRARIES += libasn1 libipcommandbus
LOCAL_SHARED_LIBRARIES += liblog liblocalconfig libtarmac libbinder libutils
LOCAL_C_INCLUDES := $(LOCAL_PATH)/infotainment_ip_service/src

include $(BUILD_EXECUTABLE)
