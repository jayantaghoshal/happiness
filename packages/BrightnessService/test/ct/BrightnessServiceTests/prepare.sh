#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

adb remount
adb shell log "before push InjectDEForBrightness"
adb push "${ANDROID_BUILD_TOP}/out/target/product/ihu_vcc/data/app/InjectDEForBrightness" "/data/local/tmp/InjectDEForBrightness"
adb shell log "after push"