LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := VtsNetmanCTCases

LOCAL_SRC_FILES := \
    netman_ct.cpp \
    netman_test_helpers.cpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_SHARED_LIBRARIES += liblog liblocalconfig

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)

#
# Copy the default JSON file to /vendor/etc
#


#
# Copy stimuli file to out structure
#
# TODO: Check with Google if we can simplify the copy of stimily files.
#


#
# Copy stimuli file to out structure
#
# TODO: Check with Google if we can simplify the copy of stimily files.
#
include $(CLEAR_VARS)
LOCAL_MODULE := netman_config_json
# Target file name
LOCAL_MODULE_STEM := DATA/localconfig_netmantest.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_ETC)

MY_STIMULI_SRC := $(LOCAL_PATH)/localconfig_netmantest.json

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts
# We may not need this.Please review ...
LOCAL_MULTILIB := 64
include $(BUILD_SYSTEM)/base_rules.mk
$(LOCAL_BUILT_MODULE) : $(MY_STIMULI_SRC)
	@echo "Copy stimuli file $< to $@"
	@mkdir -p $(dir $@)
	cp $< $@
