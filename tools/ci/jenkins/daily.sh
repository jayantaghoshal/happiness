#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

# Download from Artifactory
artifactory pull ihu_image_build "${UPSTREAM_JOB_NUMBER}" out.tgz || die "artifactory pull failed"

# Unpack out.tgz
tar xfz out.tgz || die "Unpack out.tgz failed"

# Update IHU
ihu_update || die "Failed to flash IHU image"

# Get properties
./out/host/linux-x86/bin/adb shell getprop

# Run tests
# lunch ihu_vcc-eng && vts-tradefed run commandAndExit vts --abi x86_64
# status=$?

# Push logs and reports to Artifactory
artifactory push ihu_daily_test "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/logs/*/*/*.txt.gz
artifactory push ihu_daily_test "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/results/*.zip

echo "Logs can be found at https://swf1.artifactory.cm.volvocars.biz/artifactory/webapp/#/artifacts/browse/tree/General/ICUP_ANDROID_CI/ihu_daily_test/${BUILD_NUMBER}"

# Check status
#if [ $status -ne 0 ]; then
#    die "Test failed"
#fi
