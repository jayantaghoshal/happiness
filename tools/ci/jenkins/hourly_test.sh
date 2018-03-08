#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

# Download from Artifactory
artifactory pull ihu_hourly_build-eng "${UPSTREAM_JOB_NUMBER}" out.tgz || die "artifactory pull failed"

# Unpack out.tgz
tar xfz out.tgz || die "Unpack out.tgz failed"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

set +e
for tmp in 1 2 3
do
    # Update IHU
    ihu_update
    if [ $? -eq 0 ]; then # On success, break
        break
    elif [ $tmp -eq 3 ]; then
        die "Failed to flash IHU image" # failed after three attempts
    else
        python3 "${SCRIPT_DIR}"/ihu_ipm_reboot.py # reboot ihu on flashing failures
    fi
done
set -e

# Get properties
adb shell getprop

capability=""
if [ "${JOB_NAME}" = "ihu_hourly_test-flexray" ]
then
    capability="flexray"
    export VECTOR_FDX_IP=198.18.34.2
    export VECTOR_FDX_PORT=2809
    ping -c1 ${VECTOR_FDX_IP}
elif [ "${JOB_NAME}" = "ihu_hourly_test-audio" ]
then
    capability="audio cem"
elif [ "${JOB_NAME}" = "ihu_hourly_test-apix" ]
then
    capability="apix"
fi
export capability

clean_old_test_result_files

set +e

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=hourly -c ihu-generic adb mp-serial vip-serial ${capability} -o ${capability}
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

