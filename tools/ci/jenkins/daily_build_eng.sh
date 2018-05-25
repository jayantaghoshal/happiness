#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
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

# Add vbf tags for hourly depend on daily tag
CHARACTOR_PART="${MP_PART_NUMBER//[[:digit:]]/}"
DIGITAL_PART="${MP_PART_NUMBER//[!0-9]/}"
CHARACTOR_PART+=A
# Note: Set sw tag requires a space in between
ihuci vbf set SWL2H "$DIGITAL_PART" "$CHARACTOR_PART" --build-type eng

time make droid vts tradefed-all

# Create a generic subdirectory for build meta data files
mkdir -p -m 755 out/vcc_build_metadata
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_droid_vts_tradefed || true
cp out/.build.trace.gz out/vcc_build_metadata/make_droid_vts_tradefed_build.trace.gz || true

# Make dist (dist, OTA & VBF)
time make dist
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_dist || true
cp out/.build.trace.gz out/vcc_build_metadata/make_dist.trace.gz || true

# Build tests for daily (plan=nightly)
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan nightly staging_daily || die "Build Unit and Component tests failed"
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_tester_build || true
cp out/.build.trace.gz out/vcc_build_metadata/make_tester_build.trace.gz || true

# Create archive out.tgz
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            --directory="$REPO_ROOT_DIR" \
            ./out/target/product/ihu_abl_car/fast_flashfiles \
            ./out/target/product/ihu_abl_car/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed \
            ./out/host/linux-x86/framework
du -sh "${OUT_ARCHIVE}"

# Create archive dist.zip (zip format required to download single file within archive in Artifactory)
DIST_ARCHIVE=dist.zip
# shellcheck disable=SC1065
time (cd "${REPO_ROOT_DIR}/out" && zip -r - dist) > "${DIST_ARCHIVE}"
du -sh "${DIST_ARCHIVE}"

# Create archive ihu_update.tgz
IHU_UPDATE_ARCHIVE=ihu_update.tgz
time tar -c --use-compress-program='pigz -1' -f "${IHU_UPDATE_ARCHIVE}" \
            --directory="$REPO_ROOT_DIR" \
            ./vendor/volvocars/tools/ci/shipit
du -sh "${IHU_UPDATE_ARCHIVE}"

# Create build meta data metaData.tgz
BUILD_META_DATA=metaData.tgz
time tar -c --use-compress-program='pigz -1' -f "${BUILD_META_DATA}" \
            ./out/vcc_build_metadata || die "Could not create metadata archive"

du -sh "$BUILD_META_DATA"

# Upload to Artifactory
time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}"_"${MP_PART_NUMBER}" "${OUT_ARCHIVE}"
time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}"_"${MP_PART_NUMBER}" "${DIST_ARCHIVE}"
time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}"_"${MP_PART_NUMBER}" "${IHU_UPDATE_ARCHIVE}"
time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}"_"${MP_PART_NUMBER}" "${BUILD_META_DATA}"

# Storing part number and build number in redis. It can be used as lookup table.
redis-cli set icup_android.jenkins."${JOB_NAME}"."${BUILD_NUMBER}".mp_part_number "${MP_PART_NUMBER}" || die "redis-cli set failed"

# Cleanup
rm "${OUT_ARCHIVE}"
rm "${DIST_ARCHIVE}"
rm "${IHU_UPDATE_ARCHIVE}"
rm "${BUILD_META_DATA}"
