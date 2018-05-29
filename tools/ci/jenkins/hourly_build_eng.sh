#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex

# Parameter for selecting dryrun test - controls the need for repo sync, which is needed for an open change in Gerrit.
# Default is ordinary hourly build.
test_type=$1

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

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

if [ "${test_type}" == "dryrun" ]; then
   # Rerun commit check in case merge effect changed after the change was validated at the check step
   python3 ./vendor/volvocars/tools/ci/shipit/bump.py . check "${ZUUL_BRANCH}"

   # Update the manifests based on the templates and download all other
   # repositories. First time this will take a very long time but subsequent
   # downloads are incremental and faster.
   citime python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local sync "${ZUUL_PROJECT}"
fi

rm -rf out  # Remove previous OUT_DIR for clean build.

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

if [ "${test_type}" == "dryrun" ]; then
   # Rerun commit check in case it changed after the change was validated at the check step
   time "$SCRIPT_DIR"/commit_check_and_gate_common.sh
   time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/bin/gate_check.py
fi

# Add part number for hourly build
MP_PART_NUMBER=$(ihuci vbf get SWL2H)
export MP_PART_NUMBER
ihuci vbf inc SWL2H # Increment part number suffix

# Build image & tradefed
time make droid tradefed-all

# Create a generic subdirectory for build meta data files
mkdir -p -m 755 out/vcc_build_metadata
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_droid_and_tradefed_all || true
cp out/.build.trace.gz out/vcc_build_metadata/make_droid_and_tradefedall.trace.gz || true


# Build vendor/volovcar tests (Unit and Component Tests)
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan hourly staging_hourly || die "Build Unit and Component tests failed"
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_tester_build || true
cp out/.build.trace.gz out/vcc_build_metadata/make_tester_build.trace.gz || true

# Create archive out.tgz
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            ./out/target/product/ihu_abl_car/fast_flashfiles \
            ./out/target/product/ihu_abl_car/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed \
            ./out/host/linux-x86/framework || die "Could not create out archive"

du -sh "$OUT_ARCHIVE"

# Upload to Artifactory
if [ "${test_type}" == "dryrun" ]; then
   citime scp "${OUT_ARCHIVE}" jenkins@artinfcm.volvocars.net:/home/jenkins/archive/"${JOB_NAME}"/"${ZUUL_COMMIT}"
else
   time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}"_"${MP_PART_NUMBER}" "${OUT_ARCHIVE}" || die "Could not push out archive to Artifactory."
fi
redis-cli set icup_android.jenkins."${JOB_NAME}"."${BUILD_NUMBER}".mp_part_number "${MP_PART_NUMBER}" || die "redis-cli set failed"
redis-cli set icup_android.jenkins."${JOB_NAME}"."${BUILD_NUMBER}".commit "${UPSTREAM_JOB_GIT_REVISION}" || die "redis-cli set failed"

# Set this job to latest image build in Redis
redis-cli set icup_android.jenkins."${JOB_NAME}".latest.job_number "${BUILD_NUMBER}" || die "Failed to set LATEST image build in Redis"

# Create build meta data metaData.tgz
BUILD_META_DATA=metaData.tgz
time tar -c --use-compress-program='pigz -1' -f "${BUILD_META_DATA}" \
            ./out/vcc_build_metadata  || die "Could not create metadata archive"

ls -lh "$BUILD_META_DATA"

if [ "${test_type}" == "dryrun" ]; then
   time scp "${BUILD_META_DATA}" jenkins@artinfcm.volvocars.net:/home/jenkins/archive/"${JOB_NAME}"/"${ZUUL_COMMIT}"
else
   time artifactory push "${JOB_NAME}" "${BUILD_NUMBER}"_"${MP_PART_NUMBER}" "${BUILD_META_DATA}" || die "Could not push out archive to Artifactory."
   time python3 vendor/volvocars/tools/ci/shipit/report_job_status.py -j "${JOB_NAME}" -g "${GIT_COMMIT}" -s pass || die "Report job status failed"
fi


# Cleanup
rm ${OUT_ARCHIVE}
