# Only include this folder for VCC targets (or other upstream targets will suffer)
ifneq ($(filter ihu_vcc, $(TARGET_DEVICE)),)
  LOCAL_PATH:= $(call my-dir)

  LOCAL_PATH_MAKEFILES := $(call all-makefiles-under,$(LOCAL_PATH))

  # HMI and APPS, top level makefiles
  IHU_VCC_HMI_MAKEFILES := $(call first-makefiles-under,$(LOCAL_PATH)/hmi)
  IHU_VCC_APPS_MAKEFILES := $(call first-makefiles-under,$(LOCAL_PATH)/apps)

  VENDOR_VOLVOCARS_ROOT_PATH:=$(LOCAL_PATH)
  VENDOR_VOLVOCARS_NATIVE_MODULE_SETTINGS:=$(VENDOR_VOLVOCARS_ROOT_PATH)/defaults/CommonNativeModuleSettings.mk
  VENDOR_VOLVOCARS_NATIVE_MODULE_SETTINGS_HQ:=$(VENDOR_VOLVOCARS_ROOT_PATH)/defaults/CommonNativeModuleSettingsHq.mk

  # Include directive must be last, after the above call functions
  include $(LOCAL_PATH_MAKEFILES) $(IHU_VCC_HMI_MAKEFILES) $(IHU_VCC_APPS_MAKEFILES)
endif
