#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

adb remount
adb push "${ANDROID_BUILD_TOP}/out/target/product/ihu_vcc/system/bin/InjectFlexRayIllumination" "/data/local/tmp/InjectFlexRayIllumination"
