#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Build image
docker_run "lunch ihu_vcc-eng && make -j16 droid" || die "Build image failed"

# Build vts-tradefed
docker_run "lunch ihu_vcc-eng && make -j16 vts" || die "Build VTS failed"

# Create archive out.tgz 
docker_run "tar cvfz out.tgz \
            ./out/target/product/ihu_vcc/fast_flashfiles/ \
            ./out/host/linux-x86/bin/fastboot \
            ./out/host/linux-x86/bin/adb \
            ./out/host/linux-x86/bin/aapt \
            ./out/host/linux-x86/vts/android-vts" || die "Archive out files failed"

# Upload to Artifactory
docker_run "artifactory push ihu_image_build ${BUILD_NUMBER} out.tgz" || die "Upload out.tgz to Artifactory failed"
docker_run "redis-cli set icup_android.jenkins.ihu_image_build.${BUILD_NUMBER}.commit ${GIT_COMMIT}" || die "redis-cli set failed"
