# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Inherit Delphi's product configuration.
$(call inherit-product, device/delphi/volvoihu/ihu_abl_car/ihu_abl_car.mk)

# Inherit our product configuration.
$(call inherit-product, $(LOCAL_PATH)/device.mk)

# Override PRODUCT variables
PRODUCT_NAME := ihu_vcc
PRODUCT_DEVICE := ihu_vcc

# Resource overlays
PRODUCT_PACKAGE_OVERLAYS := $(LOCAL_PATH)/overlay $(PRODUCT_PACKAGE_OVERLAYS)
