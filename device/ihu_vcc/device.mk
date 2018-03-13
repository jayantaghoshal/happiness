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
    desip_service \
    android.hardware.automotive.vehicle.vcc@2.0-service \
    vendor.volvocars.hardware.uds@1.0-service \
    vendor.volvocars.hardware.profiles@1.0-impl \
    soundnotifications \
    vendor.volvocars.hardware.settingsstorage@1.0-service \
    installationmasterd \

#############################################################
# VCC System Apps/Services
#############################################################
PRODUCT_PACKAGES += \
    TimeUpdateService \
    LcfService

##############################################################
# VIP update VBFs
##############################################################
VIP_VBF_IMAGES_PATH := vendor/delphi/vcc_ihu/common/vip_images/ihu_abl_car/proprietary
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,$(VIP_VBF_IMAGES_PATH),vendor/vip-update)

##############################################################
# TODO remove this temporary hack
# lines belowe were moved between swdl bb_reprogramming and device release repo.
##############################################################
MP_PART_NUMBER ?= 00000000DEV
ENABLE_AUTO_FLASHING ?= 1

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.build.mp.part_number=$(MP_PART_NUMBER) \
    persist.swdl.EnableAutoFlashing=$(ENABLE_AUTO_FLASHING) \

##############################################################
# System UI
##############################################################
PRODUCT_PACKAGES += \
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
    BrightnessService \

##############################################################
# Internal Apps
##############################################################
PRODUCT_PACKAGES += \
    VccUserSwitchService

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
    vendor/volvocars/device/ihu_vcc/init.rc:root/init.${TARGET_PRODUCT}.rc \
    vendor/volvocars/device/ihu_vcc/init.recovery.rc:root/init.recovery.${TARGET_PRODUCT}.rc \
    vendor/volvocars/device/ihu_vcc/ueventd.rc:root/ueventd.${TARGET_PRODUCT}.rc

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
