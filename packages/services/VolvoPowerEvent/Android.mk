# Build the VendorVCC priv app.
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_SRC_FILES += src/com/delphi/desip/IDesip.aidl
LOCAL_SRC_FILES += src/com/delphi/desip/IDesipListener.aidl

LOCAL_RESOURCE_DIR += $(LOCAL_PATH)/res

LOCAL_AIDL_INCLUDES := $(LOCAL_PATH)/src

LOCAL_PACKAGE_NAME := VolvoPowerEvent

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_MODULE_TAGS := optional

LOCAL_MULTILIB := 64

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))