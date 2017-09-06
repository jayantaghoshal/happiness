#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

# Download from Artifactory
docker_run "artifactory pull ihu_image_build ${UPSTREAM_JOB_NUMBER} out.tgz" || die "artifactory pull failed"

# Unpack out.tgz
docker_run "tar xfz out.tgz" || die "Unpack out.tgz failed"

# Update IHU
ihu_update || die "Failed to flash IHU image"

# Get properties
docker_run "./out/host/linux-x86/bin/adb shell getprop"

# Run tests
# TODO: Create a config file with tests to run instead of calling vts-tradefed for each test
docker_run "lunch ihu_vcc-eng && ./out/host/linux-x86/vts/android-vts/tools/vts-tradefed run commandAndExit vts --abi x86_64 --module SampleShellTest"
status=$?

docker_run "lunch ihu_vcc-eng && ./out/host/linux-x86/vts/android-vts/tools/vts-tradefed run commandAndExit vts --abi x86_64 --module BinderThroughputBenchmark"
if [ $status -ne 0 ]; then
    status=$?
fi

docker_run "lunch ihu_vcc-eng && ./out/host/linux-x86/vts/android-vts/tools/vts-tradefed run commandAndExit vts --abi x86_64 --module BinderPerformanceTest"
if [ $status -ne 0 ]; then
    status=$?
fi

# Push logs and reports to Artifactory
docker_run "artifactory push ihu_hourly_test ${BUILD_NUMBER} ./out/host/linux-x86/vts/android-vts/logs/*/*/*.txt.gz"
docker_run "artifactory push ihu_hourly_test ${BUILD_NUMBER} ./out/host/linux-x86/vts/android-vts/results/*.zip"

echo "Logs can be found at https://swf1.artifactory.cm.volvocars.biz/artifactory/webapp/#/artifacts/browse/tree/General/ICUP_ANDROID_CI/ihu_hourly_test/${BUILD_NUMBER}"

# Check status
if [ $status -eq 0 ]; then
    # Mark image as LSV if hourly passed
    docker_run "redis-cli set icup_android.jenkins.ihu_image_build.lsv.job_number ${UPSTREAM_JOB_NUMBER}"
else
    die "Test failed"
fi
