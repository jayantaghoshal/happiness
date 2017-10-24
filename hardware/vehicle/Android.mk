
LOCAL_PATH := $(call my-dir)

vhal_v2_0 = android.hardware.automotive.vehicle@2.0
service_name = android.hardware.automotive.vehicle.vcc@2.0

include $(CLEAR_VARS)
LOCAL_MODULE := $(service_name)-service
LOCAL_INIT_RC := $(service_name)-service.rc
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
PACKAGES.$(LOCAL_MODULE).OVERRIDES := android.hardware.automotive.vehicle@2.0-service

SERVICE_SRC := src/VehicleHALServiceMain.cpp

LOCAL_SRC_FILES := \
     $(SERVICE_SRC)

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    liblog \
    libaudioclient \
    libutils \
    libcutils \
    libdesipclient \
    libdesip \
    libdesipservice \
    libbinder \
    $(vhal_v2_0)


LOCAL_STATIC_LIBRARIES := \
    $(vhal_v2_0)-delphi-manager-lib \
    $(vhal_v2_0)-delphi-impl-lib \
    delphi-vehicle-hal-interfaces-lib \
    libaudioparameters \
    libaudio_comms_convert

LOCAL_WHOLE_STATIC_LIBRARIES := \
    power_vehicle-hal-impl-lib \
    audio_vehicle-hal-impl-lib
     

LOCAL_CFLAGS += -Wall -Wextra -Werror

include $(BUILD_EXECUTABLE)

