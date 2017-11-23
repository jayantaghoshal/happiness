#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

# Setup ccache and put cache and Android out folder in a tmpfs.
# We need to set CC_WRAPPER and CXX_WRAPPER to explicitly use ccache version (3.2.4) in Docker
# image. The version currently shipped by Google in AOSP (3.1.9) is too old and causes build
# failures when building in parallel.
TMPFS=/dev/shm
JOB_TMPFS=$TMPFS/$JOB_NAME
export USE_CCACHE=false
export OUT_DIR=$JOB_TMPFS/out

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

rm -rf "${OUT_DIR}"  # Remove previous OUT_DIR for clean build.

# Build image, vts & tradefed 
time make -j32 droid vts tradefed-all

# Build vendor/volovcar tests (Unit and Component Tests)
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan=hourly || die "Build Unit and Component tests failed"

# Workaround broken config file that prevents vts from running
# TODO: Remove when config file is fixed
rm -f out/host/linux-x86/vts/android-vts/testcases/VtsClimateComponentTest.config

# Create archive out.tgz 
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            --directory="${JOB_TMPFS}" \
            ./out/target/product/ihu_vcc/fast_flashfiles \
            ./out/target/product/ihu_vcc/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed || die "Could not create out archive"

du -sh "$OUT_ARCHIVE"

# Upload to Artifactory
time artifactory push ihu_image_build "${BUILD_NUMBER}" "${OUT_ARCHIVE}" || die "Could not push out archive to Artifactory."
redis-cli set icup_android.jenkins.ihu_image_build."${BUILD_NUMBER}".commit "${GIT_COMMIT}" || die "redis-cli set failed"

# Set this job to latest image build in Redis
redis-cli set icup_android.jenkins.ihu_image_build.latest.job_number "${BUILD_NUMBER}" || die "Failed to set LATEST image build in Redis"

time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/report_job_status.py ihu_image_build "${BUILD_NUMBER}" pass || true

# Cleanup
rm ${OUT_ARCHIVE}
