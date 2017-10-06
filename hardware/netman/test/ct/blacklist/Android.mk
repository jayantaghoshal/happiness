LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := VtsNetdBlacklistComponentTest
VTS_CONFIG_SRC_DIR := vendor/volvocars/hardware/netman/test/ct/blacklist
include test/vts/tools/build/Android.host_config.mk

include $(CLEAR_VARS)
LOCAL_MODULE := ihuInterfaceTestJava
LOCAL_MODULE_STEM := DATA/ihuInterfaceTestAPK.apk
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_ETC)
# TODO(mejdesti): Why is this needed? It looks like a hack... ?
MY_STIMULI_SRC := $(PRODUCT_OUT)/data/app/ihuInterfaceTestAPK/ihuInterfaceTestAPK.apk

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts
# We may not need this.Please review ...
LOCAL_MULTILIB := 64
include $(BUILD_SYSTEM)/base_rules.mk
$(LOCAL_BUILT_MODULE) : $(MY_STIMULI_SRC)
	@echo "Copy stimuli file $< to $@"
	@mkdir -p $(dir $@)
	cp $< $@




