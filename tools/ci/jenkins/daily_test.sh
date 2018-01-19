#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

# Download from Artifactory
artifactory pull ihu_daily_build_vcc_eng "${UPSTREAM_JOB_NUMBER}" out.tgz || die "artifactory pull failed"

# Unpack out.tgz
tar xfz out.tgz || die "Unpack out.tgz failed"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Update IHU
ihu_update || die "Failed to flash IHU image"

# Get properties
adb shell getprop

capability=""
if [ "${JOB_NAME}" = "ihu_daily_test-flexray" ]
then
    capability="flexray"
elif [ "${JOB_NAME}" = "ihu_daily_test-audio" ]
then
    capability="audio"
elif [ "${JOB_NAME}" = "ihu_daily_test-apix" ]
then
    capability="apix"
fi
export capability

clean_old_test_result_files

set +e

# time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=hourly -c ihu-generic adb mp-serial vip-serial ${capability} -o ${capability}
# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=nightly -c ihu-generic adb mp-serial vip-serial ${capability} -o ${capability}
status=$?

set -e

collect_test_result_files

# Push logs and reports to Artifactory
artifactory push "${JOB_NAME}" "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/logs/*/*/*.txt.gz
artifactory push "${JOB_NAME}" "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/results/*.zip

echo "Logs can be found at https://swf1.artifactory.cm.volvocars.biz/artifactory/webapp/#/artifacts/browse/tree/General/ICUP_ANDROID_CI/${JOB_NAME}/${BUILD_NUMBER}"

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi