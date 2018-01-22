# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#
# Copyright (C) 2017 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#$(call inherit-product, device/generic/car/common/car.mk)
$(call inherit-product, $(LOCAL_PATH)/car/common/car.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_x86.mk)

# Inherit our product configuration.
$(call inherit-product, $(LOCAL_PATH)/device.mk)

PRODUCT_NAME := vcc_emulator_x86
PRODUCT_DEVICE := vcc_emulator_x86
PRODUCT_BRAND := VCC
PRODUCT_MODEL := VCC on x86 emulator


ALLOW_MISSING_DEPENDENCIES=true



BUILD_FINGERPRINT := $(PRODUCT_BRAND)/vcc_emu_x86/$(TARGET_DEVICE):$(PLATFORM_VERSION)/$(BUILD_ID)/$(BF_BUILD_NUMBER):$(TARGET_BUILD_VARIANT)/$(BUILD_VERSION_TAGS)
