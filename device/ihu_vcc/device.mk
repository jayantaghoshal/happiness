
##############################################################
# Native daemons
##############################################################
PRODUCT_PACKAGES += \
    netman \
    netboyd \
    vehicle-signals-daemon \
    dataelements-hidl-server \
    ipcbd \
    iplmd \
    gnssd

##############################################################
# System UI
##############################################################
PRODUCT_PACKAGES += \
    VolvoLauncher

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
# Configurations for userdebug and engineering build
##############################################################
ifneq (,$(filter userdebug eng, $(TARGET_BUILD_VARIANT)))
PRODUCT_PACKAGES += \
	signaltrace \
	signalecho
endif

##############################################################
# VCC required Linux kernel modules
##############################################################
KERNEL_DIFFCONFIG += vendor/volvocars/device/ihu_vcc/kernel_configs/vcc_connectivity_diffconfig
