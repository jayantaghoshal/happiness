
##############################################################
# Native daemons
##############################################################
PRODUCT_PACKAGES += \
    netman \
    infotainment-ip-service

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
