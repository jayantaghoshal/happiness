# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := HappyGetPendingInstallations

LOCAL_MODULE_TAGS := optional

LOCAL_REQUIRED_MODULES := softwareupdateapp

include $(BUILD_SHARED_LIBRARY)