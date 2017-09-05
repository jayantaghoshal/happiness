LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= dataelements-hidl-server
LOCAL_INIT_RC := dataelements-hidl-server.rc
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true

LOCAL_SRC_FILES:= src/main.cpp

LOCAL_SHARED_LIBRARIES += libc++ \
    liblog \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    libdataelements \
    ihu.signals@1.0 \
    libutils \
    android.hidl.base@1.0

LOCAL_CPPFLAGS := -Wno-non-virtual-dtor \
                  -fexceptions\
                  -Wno-unused-parameter \
                  -Wno-macro-redefined


LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/src 
  
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

include $(BUILD_EXECUTABLE)
