#!/bin/bash

# It updates/generates the make files for vendor/volvocars/interfaces
# Run this file from root of Android project. e.g.
# cd ~/icup_android/
# ./vendor/volvocars/interfaces/update-makefiles.sh

source system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update \
  "vendor.volvocars.hardware:vendor/volvocars/interfaces" \
  "android.hidl:system/libhidl/transport" \
  "android.hardware:hardware/interfaces"