# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Be aware that to override files in PRODUCT_COPY_FILES, files
# need to be added _FIRST_ in PRODUCT_COPY_FILES list as the
# first instance in the target filesystem is copied and all
# other that could potentially overwrite the same target
# file are ignored.

##############################################################
# Files to deploy to target
#############################################l#################
PRODUCT_COPY_FILES += \
    ${VCC_DEVICE_PATH}/init.rc:vendor/etc/init/init.${TARGET_PRODUCT}.rc \
    ${VCC_DEVICE_PATH}/init.recovery.rc:vendor/etc/init/init.recovery.${TARGET_PRODUCT}.rc \

##############################################################
# Aptiv init.rc file overrides
##############################################################
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/init_rc_files/init.delphi.vcc_ihu.doip_router.rc:root/init.delphi.vcc_ihu.doip_router.rc \

##############################################################
# Hardware permissions on target
##############################################################
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml

##############################################################
# Software permissions on target
##############################################################
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.software.activities_on_secondary_displays.xml:system/etc/permissions/android.software.activities_on_secondary_displays.xml

##############################################################
# alsa_arecord, used for audio tests in CI
# This is a TEMPORARY workaround until alsa_arecord are
# added/restored to the image by aptive.
##############################################################
PRODUCT_COPY_FILES += \
    vendor/volvocars/tools/lib/python/audio/alsa_arecord_omr1:vendor/bin/alsa_arecord_omr1

##############################################################
# Set white-listning of packages
##############################################################
PRODUCT_COPY_FILES += $(LOCAL_PATH)/extra_files/platformxml/platform.xml:system/etc/permissions/vcc_platform.xml

