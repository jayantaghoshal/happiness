# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

vhal_v2_0 = android.hardware.automotive.vehicle@2.0
vccvhal_v1_0 = vendor.volvocars.hardware.vehiclehal@1.0
audiohal_v1_0 = vendor.delphi.audiomanager@1.0
service_name = android.hardware.automotive.vehicle.vcc@2.0

include $(CLEAR_VARS)
LOCAL_MODULE := $(service_name)-service
LOCAL_INIT_RC := $(service_name)-service.rc
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
PACKAGES.$(LOCAL_MODULE).OVERRIDES := android.hardware.automotive.vehicle.delphi@2.0-service

LOCAL_SRC_FILES := \
     src/VehicleHALServiceMain.cpp \
     src/carconfigmodule.cpp \
     src/hvacmodule.cpp \
     src/fanlevelimpl.cpp \
     src/tempimpl.cpp \
     src/keymanagermodule.cpp \
     src/homebuttonmodule.cpp \
     src/activeuserprofilemodule.cpp \
     src/modulepropertyhandler.cpp \
     src/propertyhandler.cpp \
     src/papropertyhandler.cpp \
     src/systeminformationmodule.cpp \
     src/illuminationmodule.cpp \
     src/cartimemodule.cpp \
     src/sensormodule.cpp \
     src/connectedsafety.cpp \
     src/activesafetymodule.cpp \
     src/daisettingimpl.cpp \
     src/interface_mapping.cpp \
     vhal_modules/curve_speed_adaption_module.cpp \
     vhal_modules/speed_limit_adaptation_module.cpp \
     vhal_modules/lane_keeping_aid_module.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/vhal_modules

# TODO (Abhi) ugly hack to get symbols resolved. Ideally Delphi should use LOCAL_WHOLE_STATIC_LIBRARIES while building
# audio_vehicle-hal-impl-lib. Delphi is informed
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
    $(vhal_v2_0) \
    $(vccvhal_v1_0) \
    $(audiohal_v1_0) \
    libcarconfig_reader \
    libtarmac \
    libdataelements \
    libtraceclient \
    libAudioManagerBinder \
    libAudioManagerCallbackBinder \
    liblocalconfig \
    libsettings \
    libihulog

LOCAL_STATIC_LIBRARIES := \
    $(vhal_v2_0)-delphi-manager-lib \
    $(vhal_v2_0)-delphi-impl-lib \
    delphi-vehicle-hal-interfaces-lib \
    libaudioparameters \
    libaudio_comms_convert \
    libccparameterlistgen \
    climate_main \
    climate_firstrow \
    climate_signalproxy \
    climate_user_selection \
    climate_common \
    climate_vf_common \
    climate_commonapi_printers \
    climate_commonapi_pregen

LOCAL_WHOLE_STATIC_LIBRARIES := \
    power_vehicle-hal-impl-lib \
    audio_vehicle-hal-impl-lib

include $(VENDOR_VOLVOCARS_NATIVE_MODULE_SETTINGS_HQ)

include $(BUILD_EXECUTABLE)
include $(call all-makefiles-under,$(LOCAL_PATH))
