# This file is autogenerated by hidl-gen. Do not edit manually.

LOCAL_PATH := $(call my-dir)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.volvocars.hardware.uds-V1.0-java
LOCAL_MODULE_CLASS := JAVA_LIBRARIES

intermediates := $(call local-generated-sources-dir, COMMON)

HIDL := $(HOST_OUT_EXECUTABLES)/hidl-gen$(HOST_EXECUTABLE_SUFFIX)

LOCAL_JAVA_LIBRARIES := \
    android.hidl.base-V1.0-java \


#
# Build types.hal (DidReadResult)
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/DidReadResult.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::types.DidReadResult

$(GEN): $(LOCAL_PATH)/types.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build types.hal (DidReadStatusCode)
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/DidReadStatusCode.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::types.DidReadStatusCode

$(GEN): $(LOCAL_PATH)/types.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build IUdsDataCollector.hal
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/IUdsDataCollector.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IUdsDataCollector.hal
$(GEN): PRIVATE_DEPS += $(LOCAL_PATH)/IUdsDataProvider.hal
$(GEN): $(LOCAL_PATH)/IUdsDataProvider.hal
$(GEN): PRIVATE_DEPS += $(LOCAL_PATH)/types.hal
$(GEN): $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::IUdsDataCollector

$(GEN): $(LOCAL_PATH)/IUdsDataCollector.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build IUdsDataProvider.hal
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/IUdsDataProvider.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IUdsDataProvider.hal
$(GEN): PRIVATE_DEPS += $(LOCAL_PATH)/types.hal
$(GEN): $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::IUdsDataProvider

$(GEN): $(LOCAL_PATH)/IUdsDataProvider.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)
include $(BUILD_JAVA_LIBRARY)


################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.volvocars.hardware.uds-V1.0-java-static
LOCAL_MODULE_CLASS := JAVA_LIBRARIES

intermediates := $(call local-generated-sources-dir, COMMON)

HIDL := $(HOST_OUT_EXECUTABLES)/hidl-gen$(HOST_EXECUTABLE_SUFFIX)

LOCAL_STATIC_JAVA_LIBRARIES := \
    android.hidl.base-V1.0-java-static \


#
# Build types.hal (DidReadResult)
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/DidReadResult.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::types.DidReadResult

$(GEN): $(LOCAL_PATH)/types.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build types.hal (DidReadStatusCode)
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/DidReadStatusCode.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::types.DidReadStatusCode

$(GEN): $(LOCAL_PATH)/types.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build IUdsDataCollector.hal
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/IUdsDataCollector.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IUdsDataCollector.hal
$(GEN): PRIVATE_DEPS += $(LOCAL_PATH)/IUdsDataProvider.hal
$(GEN): $(LOCAL_PATH)/IUdsDataProvider.hal
$(GEN): PRIVATE_DEPS += $(LOCAL_PATH)/types.hal
$(GEN): $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::IUdsDataCollector

$(GEN): $(LOCAL_PATH)/IUdsDataCollector.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build IUdsDataProvider.hal
#
GEN := $(intermediates)/vendor/volvocars/hardware/uds/V1_0/IUdsDataProvider.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IUdsDataProvider.hal
$(GEN): PRIVATE_DEPS += $(LOCAL_PATH)/types.hal
$(GEN): $(LOCAL_PATH)/types.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.volvocars.hardware:vendor/volvocars/interfaces \
        vendor.volvocars.hardware.uds@1.0::IUdsDataProvider

$(GEN): $(LOCAL_PATH)/IUdsDataProvider.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)
include $(BUILD_STATIC_JAVA_LIBRARY)



include $(call all-makefiles-under,$(LOCAL_PATH))
