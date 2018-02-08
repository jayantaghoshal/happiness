# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Inherit from Delphi's BoardConfig
include device/delphi/volvoihu/ihu_abl_car/BoardConfig.mk

BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive

# TODO Enable enforcing once policies are updated

# content inspired by system/sepolicy/public - abstract hal / services declarations
BOARD_SEPOLICY_DIRS += vendor/volvocars/device/ihu_vcc/sepolicy_hal_decl

# concrete VCC implementation based on types provided in sepolicy_hal_decl
BOARD_SEPOLICY_DIRS += vendor/volvocars/device/ihu_vcc/sepolicy

TARGET_FS_CONFIG_GEN += vendor/volvocars/device/ihu_vcc/common/filesystem_config/config.fs

DEVICE_MANIFEST_FILE += vendor/volvocars/device/ihu_vcc/manifest.xml
