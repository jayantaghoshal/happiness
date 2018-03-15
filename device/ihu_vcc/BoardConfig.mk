# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

VCC_DEVICE_PATH := vendor/volvocars/device/ihu_vcc/

##############################################################
# SELinux policies
#############################################################
BOARD_SEPOLICY_DIRS += \
    $(VCC_DEVICE_PATH)/sepolicy/vcc_hal_decl \
    $(VCC_DEVICE_PATH)/sepolicy/vcc

##############################################################
# Manifest
#############################################################
DEVICE_MANIFEST_FILE +=$(VCC_DEVICE_PATH)/manifest.xml

##############################################################
# FS Config
#############################################################
TARGET_FS_CONFIG_GEN := $(TARGET_FS_CONFIG_GEN) $(VCC_DEVICE_PATH)/common/filesystem_config/config.fs