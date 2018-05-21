# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##############################################################
# Disabled standard System Services (see SystemServer.java)
##############################################################
PRODUCT_PROPERTY_OVERRIDES += \
    config.disable_networktime=1

##############################################################
# Native daemons
##############################################################
PRODUCT_PACKAGES += \
    var_run_directory_structure \
    netboyd \
    netmand \
    cloudd \
    mqttd \
    vehicle-signals-daemon \
    dataelements-hidl-server \
    ipcbd \
    iplmd \
    dimmanager \
    gnssd \
    carconfig-updater \
    carconfig2prop \
    desip_service \
    android.hardware.automotive.vehicle.vcc@2.0-service \
    vendor.volvocars.hardware.uds@1.0-service \
    vendor.volvocars.hardware.profiles@1.0-impl \
    soundnotifications \
    vendor.volvocars.hardware.settingsstorage@1.0-service \
    installationmasterd \
    android.hardware.automotive.evs.vcc@1.0-hardware \
    android.hardware.automotive.evs.vcc@1.0-manager \
    pac_early_app \
    remotectrl_routerd \
    remotectrl_audioctrld \
    remotectrl_climatectrld \
    remotectrl_mediactrld \
    remotectrl_generalsettingsctrld \
    connectivitymanagerd \
    iplm_ctrl

PRODUCT_PROPERTY_OVERRIDES += \
    persist.vcc.service.evs.boot=true

#############################################################
# VCC System Apps/Services
#############################################################
PRODUCT_PACKAGES += \
    TimeUpdateService \
    TimeZoneService \
    LcfService \
    timezones.db \
    libjsqlite \
    ConManGw \
    ConManRelay \
    RemoteAudioCtrlService \
    RemoteAudioCtrlAppService \
    RemoteClimateGateway \
    RemoteClimateService \
    RemoteMediaCtrlGateway \
    RemoteMediaCtrlService \
    RemoteCtrlGeneralSettingsGateway \
    RemoteCtrlGeneralSettingsService \

##############################################################
# SWDL stuff
##############################################################
MP_PART_NUMBER ?= 00000000DEV
ENABLE_AUTO_FLASHING := 1

PRODUCT_PROPERTY_OVERRIDES += \
    ro.build.mp.part_number=$(MP_PART_NUMBER) \
    persist.swdl.EnableAutoFlashing=$(ENABLE_AUTO_FLASHING)

##############################################################
# System UI
#
# TODO: The librs_jni package is needed by HMI and added here
#       explicitly. It is being pulled in by some other
#       package in master, but since trying to integrate
#       Aptivs Kraken product config, it seems to be missing.
##############################################################
PRODUCT_PACKAGES += \
    librs_jni \
    VolvoLauncher \
    VccLauncher \
    CarLatinIME \
    SystemDialog

##############################################################
# Volvo system fonts
##############################################################
PRODUCT_PACKAGES += \
    instrument205.otf \
    volvosansloballight.ttf \
    volvosansglobalmedium.ttf \
    volvosansgloballight.ttf

ADDITIONAL_FONTS_FILE := vendor/volvocars/hmi/ComponentLibrary/uxlibrary/system_fonts

##############################################################
# Internal Apps UI
##############################################################
ifneq (,$(filter userdebug eng, $(TARGET_BUILD_VARIANT)))
PRODUCT_PACKAGES += \
    HalModulesSink
endif

PRODUCT_PACKAGES += \
    BrightnessService

##############################################################
# Internal Apps
##############################################################
PRODUCT_PACKAGES += \
    VccUserSwitchService \
    VehicleFunctions \
    VccMediaCenter \
    VccTunerBrowserService \
    VccSettings

##############################################################
# Vendor apps
##############################################################
PRODUCT_PACKAGES += \
    Spotify

##############################################################
# Vendor priv-apps (OTA and Cloud)
##############################################################
PRODUCT_PACKAGES += \
    CloudService \
    SoftwareUpdateService \
    SettingsStorageService

##############################################################
# Configurations for userdebug and engineering build
##############################################################
ifneq (,$(filter userdebug eng, $(TARGET_BUILD_VARIANT)))
PRODUCT_PACKAGES += \
    signaltrace \
    vhaltrace \
    signalecho \
    carsim_bridge \
    signal_toggle_sender \
    carconfig-client-tester \
    hisipcmd \
    apix_setup_sock_util \
    changecarconfig \
    readcarconfig
endif

##############################################################
# Configurations for engineering build only
##############################################################
ifneq (,$(filter eng, $(TARGET_BUILD_VARIANT)))
PRODUCT_PACKAGES += \
    execns
endif

##############################################################
# Google Services
##############################################################

# Call gms makefile in vendor/google/apps/GAS to include all
# everything in GAS delivery
$(call inherit-product, vendor/volvocars/apps/google/GAS/google/products/gms.mk)

# Required by Google Assistant
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opa.eligible_product=true

##############################################################
# VCC required Linux kernel modules
##############################################################
KERNEL_DIFFCONFIG += ${VCC_DEVICE_PATH}/kernel_configs/vcc_connectivity_diffconfig

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
    KERNEL_DIFFCONFIG += ${VCC_DEVICE_PATH}/kernel_configs/vcc_connectivity_eng_diffconfig
endif

##############################################################
