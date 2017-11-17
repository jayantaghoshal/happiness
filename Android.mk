LOCAL_PATH:= $(call my-dir)

LOCAL_PATH_MAKEFILES := $(call all-makefiles-under,$(LOCAL_PATH))

# HMI and APPS, top level makefiles
IHU_VCC_HMI_MAKEFILES := $(call first-makefiles-under,$(LOCAL_PATH)/hmi)
IHU_VCC_APPS_MAKEFILES := $(call first-makefiles-under,$(LOCAL_PATH)/apps)

# Include directive must be last, after the above call functions
include $(LOCAL_PATH_MAKEFILES) $(IHU_VCC_HMI_MAKEFILES) $(IHU_VCC_APPS_MAKEFILES)
