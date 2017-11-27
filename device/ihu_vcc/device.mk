##############################################################
# Native daemons
##############################################################
PRODUCT_PACKAGES += \
    var_run_directory_structure \
    netboyd \
    netman \
    vehicle-signals-daemon \
    dataelements-hidl-server \
    ipcbd \
    iplmd \
    gnssd \
    carconfig-updater \
    desip_service \
    android.hardware.automotive.vehicle.vcc@2.0-service \
    vendor.volvocars.hardware.uds@1.0-service


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
    VccLauncher

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
    signalecho \
    carsim_bridge \
    signal_toggle_sender \
    carconfig-client-tester \
    hisipcmd
endif

##############################################################
# VCC required Linux kernel modules
##############################################################
KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_diffconfig

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
    KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_eng_diffconfig
endif
