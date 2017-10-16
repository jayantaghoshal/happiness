LOCAL_PATH := $(call my-dir)

MY_LOCAL_C_INCLUDES=$(LOCAL_PATH)/include
MY_LOCAL_EXPORT_C_INCLUDE_DIRS=$(MY_LOCAL_CONFIG_C_INCLUDES)
MY_LOCAL_CPPFLAGS := -Wno-non-virtual-dtor -fexceptions -Wno-unused-parameter -Wno-macro-redefined

#
# Netman executable for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := netman
LOCAL_SRC_FILES := \
	src/netman/netman.cpp \
	src/netutils.cpp \
	src/netlink_event_listener.cpp \
	src/netlink_event_handler.cpp \
	src/netman/netman_netlink_event_handler.cpp \
	src/netman/firewall_config.cpp

LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/netman/

LOCAL_CPPFLAGS := $(MY_LOCAL_CPPFLAGS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_EXPORT_C_INCLUDE_DIRS)
LOCAL_SHARED_LIBRARIES += \
	liblog \
	liblocalconfig \
	libcutils

LOCAL_STATIC_LIBRARIES += \
	libbase

# Netman depends upon sysctl.conf, which it will initialize during startup
LOCAL_INIT_RC := netman.rc
LOCAL_REQUIRED_MODULES := sysctl.conf netd_config.xml
# We only build for 64 bit.
LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)


#
# Netboyd executable for deployment to system
#
include $(CLEAR_VARS)
LOCAL_MODULE := netboyd
LOCAL_SRC_FILES := \
	src/netboy/netboy.cpp \
	src/netutils.cpp \
	src/netlink_event_listener.cpp \
	src/netlink_event_handler.cpp \
	src/netboy/netboy_netlink_event_handler.cpp

LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/netboy/

LOCAL_CPPFLAGS := $(MY_LOCAL_CPPFLAGS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_EXPORT_C_INCLUDE_DIRS)
LOCAL_SHARED_LIBRARIES += \
	liblog \
	liblocalconfig \
	libcutils

LOCAL_STATIC_LIBRARIES += \
	libbase

LOCAL_INIT_RC := netboyd.rc
# We only build for 64 bit.
LOCAL_MULTILIB := 64
include $(BUILD_EXECUTABLE)

#
# Copy sysctl.conf to /vendor/etc/
#
include $(CLEAR_VARS)
LOCAL_MODULE := sysctl.conf
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := conf/sysctl.conf
# This is probably not needed, please review...
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)

#
# Copy conf/netd_config.xml to /system/etc/netd
#
include $(CLEAR_VARS)
LOCAL_MODULE := netd_config.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_RELATIVE_PATH := netd
LOCAL_SRC_FILES := conf/netd_config.xml
include $(BUILD_PREBUILT)

include $(call all-makefiles-under,$(LOCAL_PATH))
