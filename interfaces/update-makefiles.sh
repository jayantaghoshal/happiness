#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# It updates/generates the make files for vendor/volvocars/interfaces
# Run this file from root of Android project. e.g.
# cd ~/icup_android/
# ./vendor/volvocars/interfaces/update-makefiles.sh

interfacepath="vendor/volvocars/interfaces"

source system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update \
  "vendor.volvocars.hardware:$interfacepath" \
  "android.hidl:system/libhidl/transport" \
  "android.hardware:hardware/interfaces"

# Since do_makefiles_update/hidl-gen creates .bp-files that does not match our
# formatting requirements we also run the formatter on the created .bp-files
find "$interfacepath" -name Android.bp -print0 | xargs -0 bpfmt -w