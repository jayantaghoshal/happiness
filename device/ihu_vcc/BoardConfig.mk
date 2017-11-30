# Inherit from Delphi's BoardConfig
include device/delphi/volvoihu/ihu_abl_car/BoardConfig.mk

BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive

# TODO Enable once policies are updated
BOARD_SEPOLICY_DIRS += \
	vendor/volvocars/device/ihu_vcc/sepolicy

DEVICE_MANIFEST_FILE += vendor/volvocars/device/ihu_vcc/manifest.xml
