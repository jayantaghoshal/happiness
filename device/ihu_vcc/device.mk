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
TimeUpdateService


##############################################################
# Engineering SWDL
##############################################################
ifneq (,$(filter userdebug eng, $(TARGET_BUILD_VARIANT)))
PRODUCT_PACKAGES += \
    vbf_flasher \
    vbf_flashing_service
endif

PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,vendor/delphi/swdl/vip-update,vendor/vip-update)

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
    VccTestApp
endif

PRODUCT_PACKAGES += \
    VccTunerApp


##############################################################
# Vendor apps
##############################################################
PRODUCT_PACKAGES += \
    Spotify

##############################################################
# Files to deploy to target
##############################################################
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
    apix_setup_sock_util
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
    ro.control_privapp_permissions=log

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
