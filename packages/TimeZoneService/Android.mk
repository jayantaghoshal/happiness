# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Build the VendorVCC priv app.
LOCAL_PATH:= $(call my-dir)


include $(call all-makefiles-under,$(LOCAL_PATH))


# include $(CLEAR_VARS)
# LOCAL_MODULE := libjsqlite
# LOCAL_CERTIFICATE := platform
# LOCAL_PRIVILEGED_MODULE := true
# LOCAL_MULTILIB := 64

# LOCAL_MODULE_PATH_64 := system/lib64/
# LOCAL_SRC_FILES_64 := libjsqlite.so

# # ifdef TARGET_2ND_ARCH
# # LOCAL_MODULE_PATH_32 := system/lib/
# # LOCAL_SRC_FILES_32 := libjsqlite.so
# # else
# # LOCAL_SRC_FILES_64 := libjsqlite.so
# # LOCAL_MODULE_PATH_64 := system/lib64/
# # endif
# LOCAL_MODULE_CLASS := SHARED_LIBRARIES
# LOCAL_MODULE_SUFFIX := .so

# include $(BUILD_PREBUILT)

# include $(CLEAR_VARS)

# LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := mysqlite:x86_64.jar

# include $(BUILD_MULTI_PREBUILT)


include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_PACKAGE_NAME := TimeZoneService

LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_MODULE_TAGS := optional



LOCAL_STATIC_JAVA_LIBRARIES := \
        android-support-v4 \
        vendor.volvocars.hardware.vehiclehal-V1.0-java-static \
        android.hardware.automotive.vehicle-V2.0-java-static \


# LOCAL_SHARED_LIBRARIES := libjsqlite
# LOCAL_JNI_SHARED_LIBRARIES := libjsqlite

# LOCAL_MULTILIB := 32
include $(BUILD_PACKAGE)



# include $(CLEAR_VARS)
# LOCAL_MODULE := libjsqlite
# LOCAL_SRC_FILES := libjsqlite.so
# include $(PREBUILT_SHARED_LIBRARY)