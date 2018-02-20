# Copyright 2017 Volvo Car Corporation
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
    vendor.volvocars.hardware.settingsstorage@1.0-service

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
ENABLE_AUTO_FLASHING ?= 0

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
    VccTestApp \
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

GMS = vendor/volvocars/apps/google/GoogleServices

# GMS mandatory core packages
PRODUCT_PACKAGES += \
    ConfigUpdater \
    GoogleBackupTransport \
    GoogleExtServices \
    GoogleExtShared \
    GoogleFeedback \
    GoogleOneTimeInitializer \
    GooglePackageInstaller \
    GooglePartnerSetup \
    GooglePrintRecommendationService \
    GoogleServicesFramework \
    GoogleCalendarSyncAdapter \
    GoogleContactsSyncAdapter \
    GoogleTTS \
    GmsCore \
    Phonesky \
    SetupWizard \
    WebViewGoogle

# privapp-permissions whitelisting
PRODUCT_PROPERTY_OVERRIDES += \
    ro.control_privapp_permissions=log \
    config.disable_networktime=1

PRODUCT_COPY_FILES += \
    $(GMS)/etc/permissions/privapp-permissions-google.xml:system/etc/permissions/privapp-permissions-google.xml
    $(GMS)/etc/sysconfig/google.xml:system/etc/sysconfig/google.xml

# GMS mandatory application packages
# Note: Duo is mandatory for telephony devices, whereas Hangouts is for non-telephony devices.
# The following apps are not currently in the build, but can be enabled
#    Drive \
#    Gmail2 \
#    Duo \
#    Hangouts \
#    Music2 \
#    Photos \
#    Velvet \
#    Videos \
#    YouTube

PRODUCT_PACKAGES += \
    Chrome \
    Maps \
    MapsGearhead \
# TODO (Abhijeet Shirolikar) GoogleAssistant is disabled as current integrated version for audio-hal does not implement
# AUDIO_ROUTE_POLICY resulting in Car service to continuously crash. Re-enable packages below once required props are
# implemented in audio-hal
#    GoogleAssistant \
#    GoogleAssistantTextQuery

##############################################################
# VCC required Linux kernel modules
##############################################################
KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_diffconfig

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
    KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_eng_diffconfig
endif
