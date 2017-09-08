#!/bin/bash
#
# Copies the localconfig shared library to the IHU
#

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

LOCALCONFIG_SHARED_LIBRARY="${SCRIPT_DIR}/../../../../../out/target/product/ihu_vcc/obj/SHARED_LIBRARIES/liblocalconfig_intermediates/LINKED/liblocalconfig.so"
OUT_PATH="/vendor/lib64/liblocalconfig.so"

adb remount
adb push "$LOCALCONFIG_SHARED_LIBRARY" "$OUT_PATH"
adb shell "mount -o ro,remount, ro /system"
