# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                             #
#                     .-'   `'.                               #
#                    /         \                              #
#                    |         ;                              #
#                    |         |           ___.--,            #
#           _.._     |0) ~ (0) |    _.---'`__.-( (_.          #
#    __.--'`_.. '.__.\    '--. \_.-' ,.--'`     `""`          #
#   ( ,.--'`   ',__ /./;   ;, '.__.'`    __                   #
#   _`) )  .---.__.' / |   |\   \__..--""  """--.,_           #
#  `---' .'.''-._.-'`_./  /\ '.  \ _.-~~~````~~~-._`-.__.'    #
#        | |  .' _.-' |  |  \  \  '.               `~---`     #
#         \ \/ .'     \  \   '. '-._)                         #
#          \/ /        \  \    `=.__`~-.                      #
#          / /\         `) )    / / `"".`\                    #
#    , _.-'.'\ \        / /    ( (     / /                    #
#     `--~`   ) )    .-'.'      '.'.  | (                     #
#            (/`    ( (`          ) )  '-;                    #
#             `      '-;         (-'                          #
#                                                             #
#         https://www.youtube.com/watch?v=7SqC_m3yUDU         #
#                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


# RLU: treble_common.mk will pull in aosp_base.mk, which pulls in full_base.mk, which we have been
#      advised by Google not to pull in to avoid a patch. It is however difficult to avoid
#      pulling in full_base.mk...

# $(call inherit-product, $(LOCAL_PATH)/product/ihu_treble.mk)
# include build/make/target/product/treble_common.mk

BOARD_PROPERTY_OVERRIDES_SPLIT_ENABLED := true

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += ro.zygote=zygote64_32
PRODUCT_COPY_FILES += system/core/rootdir/init.zygote64_32.rc:root/init.zygote64_32.rc

BOARD_USE_64BIT_USERSPACE := true

# FIXME: Hack to get the prebuilt Intel BSP components
# CFG-WORKSHOP: Are we still expecting to fix this?
REF_PRODUCT_NAME := gordon_peak

$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)

# This must match PRODUCT_DEVICE, it is used inside product_ihu.mk to copy files that are required
# to use the device name.
MY_DEVICE_NAME=ihu_abl_car
VCC_DEVICE_PATH := vendor/volvocars/device/ihu_vcc
APTIV_KRAKEN_DEVICE_PATH := device/aptiv/kraken
include $(APTIV_KRAKEN_DEVICE_PATH)/path_config.mk

# Needed before BoardConfig.mk
APTIV_PATH_DEVICE := device/delphi/volvoihu
# Include at the end of BSP's BoardConfig.mk
PRODUCT_BOARD_CONFIG_APPEND := $(APTIV_KRAKEN_DEVICE_PATH)/BoardConfig-append.mk $(LOCAL_PATH)/BoardConfig.mk

$(call inherit-product, $(LOCAL_PATH)/device_prepend.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/core.mk)
$(call inherit-product, $(APTIV_KRAKEN_DEVICE_PATH)/kraken_product.mk)
$(call inherit-product, $(LOCAL_PATH)/device.mk)

# Resource overlays
PRODUCT_PACKAGE_OVERLAYS := $(LOCAL_PATH)/overlay $(PRODUCT_PACKAGE_OVERLAYS)

# Right now, we are dependent on having "ihu_" in the PRODUCT_NAME, because we use a substring
# of PRODUCT_NAME to differentiate between products in some components
PRODUCT_NAME := ihu_vcc
PRODUCT_DEVICE := $(MY_DEVICE_NAME)
PRODUCT_BRAND := delphi
PRODUCT_MODEL := AOSP on Delphi VolvoIHU Platform
PRODUCT_MANUFACTURER := Delphi
PRODUCT_LOCALES := en_US en_IN fr_FR it_IT es_ES et_EE de_DE nl_NL cs_CZ pl_PL ja_JP zh_TW zh_CN zh_HK ru_RU ko_KR nb_NO es_US da_DK el_GR tr_TR pt_PT pt_BR rm_CH sv_SE bg_BG ca_ES en_GB fi_FI hi_IN hr_HR hu_HU in_ID iw_IL lt_LT lv_LV ro_RO sk_SK sl_SI sr_RS uk_UA vi_VN tl_PH ar_EG fa_IR th_TH sw_TZ ms_MY af_ZA zu_ZA am_ET hi_IN en_XA ar_XB fr_CA km_KH lo_LA ne_NP mn_MN hy_AM az_AZ ka_GE de_AT de_CH de_LI en_AU en_CA en_NZ en_SG eo_EU fr_CH fr_BE it_CH nl_BE
PRODUCT_AAPT_CONFIG := normal large mdpi
PRODUCT_AAPT_PREF_CONFIG := mdpi

PRODUCT_RESTRICT_VENDOR_FILES := false
PRODUCT_SHIPPING_API_LEVEL := 26

# RLU: This does not exist, is it something we expect to find that has moved?
#$(call inherit-product-if-exists, vendor/intel/gordon_peak/device-vendor.mk)
