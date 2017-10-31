# Inherit from Delphi's AndroidBoard
include device/delphi/volvoihu/ihu_abl_car/AndroidBoard.mk

include $(CLEAR_VARS)
LOCAL_MODULE := var_run_directory_structure

LOCAL_POST_INSTALL_CMD := $(hide) mkdir -p $(TARGET_ROOT_OUT) \
    && mkdir -p $(TARGET_ROOT_OUT)/var \
    && mkdir -p $(TARGET_ROOT_OUT)/var/run \
    && ln -sf /dev/vendor/netns $(TARGET_ROOT_OUT)/var/run/netns

include $(BUILD_PHONY_PACKAGE)