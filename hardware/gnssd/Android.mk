LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := gnssd
LOCAL_INIT_RC := gnssd.rc
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

# GnssD
LOCAL_SRC_FILES := \
    src/main.cpp \
    src/gnssService.cpp

# Gnss
LOCAL_SRC_FILES += \
    src/binderimpl/gnss/AGnssCallback.cpp \
    src/binderimpl/gnss/AGnss.cpp \
    src/binderimpl/gnss/AGnssRilCallback.cpp \
    src/binderimpl/gnss/AGnssRil.cpp \
    src/binderimpl/gnss/GnssBatchingCallback.cpp \
    src/binderimpl/gnss/GnssBatching.cpp \
    src/binderimpl/gnss/GnssCallback.cpp \
    src/binderimpl/gnss/GnssConfiguration.cpp \
    src/binderimpl/gnss/Gnss.cpp \
    src/binderimpl/gnss/GnssDebug.cpp \
    src/binderimpl/gnss/GnssGeofenceCallback.cpp \
    src/binderimpl/gnss/GnssGeofencing.cpp \
    src/binderimpl/gnss/GnssMeasurementCallback.cpp \
    src/binderimpl/gnss/GnssMeasurement.cpp \
    src/binderimpl/gnss/GnssNavigationMessageCallback.cpp \
    src/binderimpl/gnss/GnssNavigationMessage.cpp \
    src/binderimpl/gnss/GnssNiCallback.cpp \
    src/binderimpl/gnss/GnssNi.cpp \
    src/binderimpl/gnss/GnssXtraCallback.cpp \
    src/binderimpl/gnss/GnssXtra.cpp


LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined
LOCAL_SHARED_LIBRARIES += \
    liblog \
    liblocalconfig \
    libtarmac \
    libdataelements \
    libutils \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    libinfotainmentbusasn \
    android.hardware.gnss@1.0 \
    vendor.volvocars.hardware.common@1.0 \
    vendor.volvocars.hardware.vehiclecom@1.0

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/gnssd/src \
    $(LOCAL_PATH)/gnssd/src/binderimpl/gnss

LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)
include $(call all-makefiles-under,$(LOCAL_PATH))