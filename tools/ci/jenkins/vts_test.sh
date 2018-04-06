#!/bin/bash

# Copyright 2018 Volvo Car Corporation
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
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"

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
        python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/ihu_ipm_reboot.py # reboot ihu on flashing failures
    fi
done
set -e

# Get properties
adb shell getprop

plan=""
if [ "${JOB_NAME}" = "ihu_daily_vts1-generic" ]
then
    plan="vts-volvo1.xml"
elif [ "${JOB_NAME}" = "ihu_daily_vts2-generic" ]
then
	plan="vts-volvo2.xml"
elif [ "${JOB_NAME}" = "ihu_daily_vts3-generic" ]
then
	plan="vts-volvo3.xml"
fi
export plan

set +e

# Run vts tests
#TODO Copy this to the above if statement as the command for cts differ a bit?
time vts-tradefed run commandAndExit "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/test/"${plan}" --skip-all-system-status-check --skip-preconditions #--abi x86_64

# Upload results to MongoDB
python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/handle_result/store_vts_result.py ./out/host/linux-x86/vts/android-vts/results/

# Compare to last run here and fail if new errors are found
mkdir old
(cd old; artifactory pull-latest "${JOB_NAME}" || echo "artifactory pull failed")
python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/compare_results.py ./out/host/linux-x86/vts/android-vts/results/ ./old
changestatus=$?

set -e

# Push reports to Artifactory
#artifactory push "${JOB_NAME}" "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/logs/*/*/*.txt.gz
artifactory push "${JOB_NAME}" "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/results/*.zip
echo "Results can be found at https://swf1.artifactory.cm.volvocars.biz/artifactory/webapp/#/artifacts/browse/tree/General/ICUP_ANDROID_CI/${JOB_NAME}/${BUILD_NUMBER}"

#check results and email relavant po's here?

# Check status
if [ $changestatus -ne 0 ]; then
    die "Test failed"
fi
