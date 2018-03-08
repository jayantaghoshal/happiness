#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

export USE_CCACHE=false

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Build image, vts & tradefed
MP_PART_NUMBER=$(ihuci vbf get SWL2)
export MP_PART_NUMBER
ihuci vbf inc SWL2 # Increment part number suffix
time make droid vts tradefed-all

# Make dist (dist, OTA & VBF)
time make dist

# Build tests for daily (plan=nightly)
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan=nightly || die "Build Unit and Component tests failed"

# Create archive out.tgz
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            --directory="$REPO_ROOT_DIR" \
            ./out/target/product/ihu_vcc/fast_flashfiles \
            ./out/target/product/ihu_vcc/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed
du -sh "${OUT_ARCHIVE}"

# Create archive dist.tgz
DIST_ARCHIVE=dist.tgz
time tar -c --use-compress-program='pigz -1' -f "${DIST_ARCHIVE}" \
            --directory="${REPO_ROOT_DIR}/out" \
            ./dist
du -sh "${DIST_ARCHIVE}"

# Create archive ihu_update.tgz
IHU_UPDATE_ARCHIVE=ihu_update.tgz
time tar -c --use-compress-program='pigz -1' -f "${IHU_UPDATE_ARCHIVE}" \
            --directory="$REPO_ROOT_DIR" \
            ./vendor/volvocars/tools/ci/shipit
du -sh "${IHU_UPDATE_ARCHIVE}"

# Upload to Artifactory
time artifactory push ihu_daily_build_vcc_eng "${BUILD_NUMBER}" "${OUT_ARCHIVE}"
time artifactory push ihu_daily_build_vcc_eng "${BUILD_NUMBER}" "${DIST_ARCHIVE}"
time artifactory push ihu_daily_build_vcc_eng "${BUILD_NUMBER}" "${IHU_UPDATE_ARCHIVE}"

# Cleanup
rm "${OUT_ARCHIVE}"
rm "${DIST_ARCHIVE}"
rm "${IHU_UPDATE_ARCHIVE}"
