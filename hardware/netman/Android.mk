# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

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

include $(CLEAR_VARS)
LOCAL_MODULE := netboyd.rules
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := conf/netboy.rules
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := netman_conf.json
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := conf/netman_conf.json
LOCAL_MULTILIB := 64
include $(BUILD_PREBUILT)
#
# Copy /system/bin/ip to /vendor/bin/ip
#
include $(CLEAR_VARS)
LOCAL_MODULE := ip-vcc
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/bin
LOCAL_REQUIRED_MODULES := ip
LOCAL_POST_INSTALL_CMD := $(hide) cp $(PRODUCT_OUT)/system/bin/ip $(TARGET_OUT_VENDOR)/bin/ip
include $(BUILD_PHONY_PACKAGE)

#
# Copy /system/bin/iptables to /vendor/bin/iptables
#
include $(CLEAR_VARS)
LOCAL_MODULE := iptables-vcc
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/bin
LOCAL_REQUIRED_MODULES := iptables
LOCAL_POST_INSTALL_CMD := $(hide) cp $(PRODUCT_OUT)/system/bin/iptables $(TARGET_OUT_VENDOR)/bin/iptables; \
	ln -sf iptables $(TARGET_OUT_VENDOR)/bin/iptables-save; \
	ln -sf iptables $(TARGET_OUT_VENDOR)/bin/iptables-restore
include $(BUILD_PHONY_PACKAGE)

#
# Copy /system/bin/ip6tables to /vendor/bin/ip6tables
#
include $(CLEAR_VARS)
LOCAL_MODULE := ip6tables-vcc
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/bin
LOCAL_REQUIRED_MODULES := ip6tables
LOCAL_POST_INSTALL_CMD := $(hide) cp $(PRODUCT_OUT)/system/bin/ip6tables $(TARGET_OUT_VENDOR)/bin/ip6tables; \
	ln -sf ip6tables $(TARGET_OUT_VENDOR)/bin/ip6tables-save; \
	ln -sf ip6tables $(TARGET_OUT_VENDOR)/bin/ip6tables-restore
include $(BUILD_PHONY_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))
