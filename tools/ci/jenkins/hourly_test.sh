#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

"${SCRIPT_DIR}/hourly_test_common.sh"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Update IHU
ihu_update || die "Failed to flash IHU image"

# Get properties
adb shell getprop

# Run tests
# TODO: Create a config file with tests to run instead of calling vts-tradefed for each test
vts-tradefed run commandAndExit vts --abi x86_64 --module SampleShellTest
status=$?

vts-tradefed run commandAndExit vts --abi x86_64 --module BinderThroughputBenchmark
result=$?
if [ $status -eq 0 ]; then
    status=$result
fi

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=hourly -c ihu-generic adb mp-serial vip-serial
result=$?
if [ $status -eq 0 ]; then
    status=$result
fi

# Push logs and reports to Artifactory
artifactory push ihu_hourly_test "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/logs/*/*/*.txt.gz
artifactory push ihu_hourly_test "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/results/*.zip

echo "Logs can be found at https://swf1.artifactory.cm.volvocars.biz/artifactory/webapp/#/artifacts/browse/tree/General/ICUP_ANDROID_CI/ihu_hourly_test/${BUILD_NUMBER}"


# Check status
if [ $status -eq 0 ]; then
    # Mark image as LSV if hourly passed
    time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/report_job_status.py ihu_hourly_test "${BUILD_NUMBER}" pass
    redis-cli set icup_android.jenkins.ihu_image_build.lsv.job_number "${UPSTREAM_JOB_NUMBER}"
else
    time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/report_job_status.py ihu_hourly_test "${BUILD_NUMBER}" fail
    die "Test failed"
fi
