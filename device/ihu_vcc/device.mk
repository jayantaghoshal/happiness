# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##############################################################
# Native daemons
##############################################################
PRODUCT_PACKAGES += \
    var_run_directory_structure \
    netboyd \
    netmand \
    cloudd \
    vehicle-signals-daemon \
    dataelements-hidl-server \
    ipcbd \
    iplmd \
    lscd \
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
    remotectrl_routerd \
    remotectrl_audioctrld \

#############################################################
# VCC System Apps/Services
#############################################################
PRODUCT_PACKAGES += \
    TimeUpdateService \
    TimeZoneService \
    LcfService \
    timezones.db \
    libjsqlite \
    RemoteAudioCtrlService \
    RemoteAudioCtrlAppService \

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
    CarLatinIME

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
    VccTunerBrowserService

##############################################################
# Vendor apps
##############################################################
PRODUCT_PACKAGES += \
    Spotify

##############################################################
# Vendor priv-apps
##############################################################
PRODUCT_PACKAGES += \
    CloudService \
    SoftwareUpdateService

##############################################################
# Files to deploy to target
#############################################l#################
PRODUCT_COPY_FILES += \
    vendor/volvocars/device/ihu_vcc/init.rc:vendor/etc/init/init.${TARGET_PRODUCT}.rc \
    vendor/volvocars/device/ihu_vcc/init.recovery.rc:vendor/etc/init/init.recovery.${TARGET_PRODUCT}.rc \

##############################################################
# Aptiv init.rc file overrides
# Be aware that to override files in PRODUCT_COPY_FILES, files
# need to be added _FIRST_ in the list as the first instance
# in the target filesystem is copied and all other that could
# potentially overwrite the same target file are ignored.
##############################################################
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/init_rc_files/init.delphi.vcc_ihu.doip_router.rc:root/init.delphi.vcc_ihu.doip_router.rc \

##############################################################
# Hardware permissions on target
##############################################################
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml

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
    changecarconfig
endif

##############################################################
# Google Services
##############################################################

# Call gms makefile in vendor/google/apps/GAS to include all
# everything in GAS delivery
$(call inherit-product, vendor/volvocars/apps/google/GAS/google/products/gms.mk)

##############################################################
# VCC required Linux kernel modules
##############################################################
KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_diffconfig

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
    KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_eng_diffconfig
endif

##############################################################
