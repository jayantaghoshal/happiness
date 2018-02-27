#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

adb remount
adb shell log "before push InjectFlexRayIllumination"
adb push "${ANDROID_BUILD_TOP}/out/target/product/ihu_vcc/data/app/InjectFlexRayIllumination" "/data/local/tmp/InjectFlexRayIllumination"
adb shell log "after push"