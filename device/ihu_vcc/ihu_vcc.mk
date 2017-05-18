#
# Copyright 2014 The Android Open Source Project
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
#

# Inherit Delphi's product configuration.
$(call inherit-product, device/delphi/volvoihu/ihu_abl_car/ihu_abl_car.mk)

# Inherit our product configuration.
$(call inherit-product, $(LOCAL_PATH)/device.mk)

# Override PRODUCT variables
PRODUCT_NAME := ihu_vcc
PRODUCT_DEVICE := ihu_vcc