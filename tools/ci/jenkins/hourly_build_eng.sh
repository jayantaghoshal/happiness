#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
VTS_REPO_HASH=$(git -C "${SCRIPT_DIR}"/../../../../../test/vts/ rev-parse HEAD)

# Setup ccache and put cache and Android out folder in a tmpfs.
# We need to set CC_WRAPPER and CXX_WRAPPER to explicitly use ccache version (3.2.4) in Docker
# image. The version currently shipped by Google in AOSP (3.1.9) is too old and causes build
# failures when building in parallel.
TMPFS=/mnt/ramdisk
export CCACHE_DIR=$TMPFS/ccache
export CCACHE_MAXSIZE=50G
export CC_WRAPPER=/usr/bin/ccache
export CXX_WRAPPER=/usr/bin/ccache
export USE_CCACHE=true

rm -rf out  # Remove previous OUT_DIR for clean build.

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Add part number for hourly build
MP_PART_NUMBER=$(ihuci vbf get SWL2H)
export MP_PART_NUMBER
ihuci vbf inc SWL2H # Increment part number suffix

# Check vts package is up to date
time checkIfVtsPackageUpToDate "$VTS_REPO_HASH"

# Build image & tradefed
time make droid tradefed-all

# Create a generic subdirectory for build meta data files
mkdir -p -m 755 out/vcc_build_metadata
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_droid_and_tradefed_all || true
cp out/.build.trace.gz out/vcc_build_metadata/make_droid_and_tradefedall.trace.gz || true

# Download VTS package from artifactory
# Note: Have tried tar xvkf --skip-old-files, tar xvkf --keep-old-files, they are not able to merge all files to make droid out/
if [ "$(checkIfVtsPackageUpToDate "$VTS_REPO_HASH")" == "rebuilt" ];then
    echo "VTS package has already been built"
else
    artifactory pull vts_build_package "$VTS_REPO_HASH" outVTS.tgz || die "Could not fetch out archive from Artifactory."
    mkdir -p -m 755 vtsPackage
    tar xvf outVTS.tgz -C vtsPackage/
    cp -r -n -a vtsPackage/out/* out/
fi

# Build vendor/volovcar tests (Unit and Component Tests)
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan hourly staging_hourly --ciflow true || die "Build Unit and Component tests failed"
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_tester_build || true
cp out/.build.trace.gz out/vcc_build_metadata/make_tester_build.trace.gz || true

# Create archive out.tgz
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            ./out/target/product/ihu_abl_car/fast_flashfiles \
            ./out/target/product/ihu_abl_car/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed || die "Could not create out archive"

du -sh "$OUT_ARCHIVE"

# Upload to Artifactory
time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}" "${OUT_ARCHIVE}" || die "Could not push out archive to Artifactory."
redis-cli set icup_android.jenkins."${JOB_NAME}"."${BUILD_NUMBER}".commit "${GIT_COMMIT}" || die "redis-cli set failed"

# Set this job to latest image build in Redis
redis-cli set icup_android.jenkins."${JOB_NAME}".latest.job_number "${BUILD_NUMBER}" || die "Failed to set LATEST image build in Redis"

# Create build meta data metaData.tgz
BUILD_META_DATA=metaData.tgz
time tar -c --use-compress-program='pigz -1' -f "${BUILD_META_DATA}" \
            ./out/vcc_build_metadata  || die "Could not create metadata archive"

ls -lh "$BUILD_META_DATA"
time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}" "${BUILD_META_DATA}" || die "Could not push out archive to Artifactory."

# Clean up vtsPackages
rm -rf vtsPackage/

rm -rf outVTS.tgz

# Cleanup
rm ${OUT_ARCHIVE}
