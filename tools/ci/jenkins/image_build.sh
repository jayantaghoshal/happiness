#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Build image
docker_run "lunch ihu_vcc-eng && make -j16 droid"
zip -r out/flash-files.zip ./out/target/product/ihu_vcc/fast_flashfiles ./out/host/linux-x86/bin/fastboot ./out/host/linux-x86/bin/adb ./out/host/linux-x86/bin/aapt

# Build vts-tradefed
docker_run "lunch ihu_vcc-eng && make -j16 vts"

# Upload to Artifactory
docker_run "artifactory push ihu_image_build ${BUILD_NUMBER} out/flash-files.zip out/host/linux-x86/vts/android-vts.zip"
