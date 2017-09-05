LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := VtsLocalConfigTestCases

LOCAL_SRC_FILES := \
    localconfig_test.cpp

# Note hat we use a static version of the liblocalconfig in the unittest as we want to decouple from
# the alread deployed shared libary in the target image.
#
# TODO: Investigate why we didn't get libjsoncpp due to transitive dependency through liblocalconfig_static.
#
LOCAL_STATIC_LIBRARIES += \
    liblocalconfig_static \
    libjsoncpp

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We only build for 64 bit.
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)

#
# Copy stimuli file to out structure
#
# TODO: Check with Google if we can simplify the copy of stimily files.
#
include $(CLEAR_VARS)
LOCAL_MODULE := localconfig_good
# Target file name
LOCAL_MODULE_STEM := DATA/localconfig_good.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_ETC)

MY_STIMULI_SRC := $(LOCAL_PATH)/localconfig_good.json

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts
# We may not need this.Please review ...
LOCAL_MULTILIB := 64
include $(BUILD_SYSTEM)/base_rules.mk
$(LOCAL_BUILT_MODULE) : $(MY_STIMULI_SRC)
	@echo "Copy stimuli file $< to $@"
	@mkdir -p $(dir $@)
	cp $< $@

#
# Copy stimuli file to out structure
#
include $(CLEAR_VARS)
LOCAL_MODULE := localconfig_bad
# Target file name
LOCAL_MODULE_STEM := DATA/localconfig_bad.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_ETC)

MY_STIMULI_SRC := $(LOCAL_PATH)/localconfig_bad.json

# Tag this module as a vts test artifact
LOCAL_COMPATIBILITY_SUITE := vts

# We may not need this.Please review ...
LOCAL_MULTILIB := 64
include $(BUILD_SYSTEM)/base_rules.mk
$(LOCAL_BUILT_MODULE) : $(MY_STIMULI_SRC)
	@echo "Copy stimuli file $< to $@"
	@mkdir -p $(dir $@)
	cp $< $@
