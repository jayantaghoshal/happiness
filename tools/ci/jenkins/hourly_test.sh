#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

"${SCRIPT_DIR}/hourly_test_common.sh"

# Get properties
docker_run "adb shell getprop"

# Run tests
# TODO: Create a config file with tests to run instead of calling vts-tradefed for each test
docker_run "lunch ihu_vcc-eng && vts-tradefed run commandAndExit vts --abi x86_64 --module SampleShellTest"
status=$?

docker_run "lunch ihu_vcc-eng && vts-tradefed run commandAndExit vts --abi x86_64 --module BinderThroughputBenchmark"
if [ $status -eq 0 ]; then
    status=$?
fi

# Run Unit and Component tests for vendor/volvocars
docker_run "time python3 $REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=hourly -c ihu-generic adb mp-serial vip-serial

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
