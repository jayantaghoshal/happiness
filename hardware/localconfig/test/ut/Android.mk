LOCAL_PATH := $(call my-dir)

localconfig_ut_SRC_FILES:= \
    local_config_parsing_test.cpp

# localconfig_test.cpp  - relies on files, to be considered to refactor with filebased unit test support.

localconfig_ut_STATIC_LIBRARIES+= \
    liblocalconfig_static \
    libjsoncpp \
    liblog

localconfig_ut_CPPFLAGS:= -fexceptions -Wall -Wextra

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE := VtsLocalConfigTestCases

LOCAL_SRC_FILES+= $(localconfig_ut_SRC_FILES)
LOCAL_STATIC_LIBRARIES += $(localconfig_ut_STATIC_LIBRARIES) libgmock
LOCAL_CPPFLAGS+= $(localconfig_ut_CPPFLAGS)

LOCAL_WHOLE_STATIC_LIBRARIES += \
    libVtsGtestMain

LOCAL_COMPATIBILITY_SUITE := vts
LOCAL_MULTILIB := 64

include $(BUILD_NATIVE_TEST)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE := vendor_volvocars_localconfig_ut

LOCAL_SRC_FILES+= $(localconfig_ut_SRC_FILES)
LOCAL_STATIC_LIBRARIES += $(localconfig_ut_STATIC_LIBRARIES) libgmock
LOCAL_CPPFLAGS+= $(localconfig_ut_CPPFLAGS)

LOCAL_MULTILIB := 64

include $(BUILD_HOST_NATIVE_TEST)

