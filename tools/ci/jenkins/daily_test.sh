#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

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

#remove old test results
rm -rf /tmp/0/ 

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=nightly -c ihu-generic adb mp-serial vip-serial
status=$?    


# Push logs and reports to Artifactory
artifactory push ihu_daily_test1 "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/logs/*/*/*.txt.gz
artifactory push ihu_daily_test1 "${BUILD_NUMBER}" ./out/host/linux-x86/vts/android-vts/results/*.zip

echo "Logs can be found at https://swf1.artifactory.cm.volvocars.biz/artifactory/webapp/#/artifacts/browse/tree/General/ICUP_ANDROID_CI/ihu_daily_test1/${BUILD_NUMBER}"

mkdir result_dir

mv "$(find /tmp/0/ -name 'test_result*.zip')" result_dir/

#unzip all
cd result_dir
for i in *.zip; do unzip "$i" -d "${i%%.zip}";mv "${i%%.zip}/*" ./"${i%%.zip}".xml; done

# Check status
if [ $status -eq 0 ]; then
    # Mark image as LSV if hourly passed
    time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/report_job_status.py ihu_daily_test1 "${BUILD_NUMBER}" pass
    
else
    time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/report_job_status.py ihu_daily_test1 "${BUILD_NUMBER}" fail
    die "Test failed"
fi

