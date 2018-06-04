#!/bin/bash -uex

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Parameter for selecting dryrun test - controls the need for repo sync, which is needed for an open change in Gerrit.
# Default is ordinary hourly build.
if [ -z "${@+x}" ]; then
    test_type="normal"
else
    test_type="$1"
fi

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
set +x
source "${SCRIPT_DIR}/common.sh"
set -x
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

if [ "$test_type" == "dryrun" ]; then
   # Update the manifests based on the templates for gate test
   time python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local no_sync "${ZUUL_PROJECT}"
   # Sync repos required for build/envsetup.sh and lunch so we can run VTS.
   repo_sync aosp/platform/build/soong aosp/platform/build/blueprint aosp/platform/prebuilts/go/linux-x86 aosp/platform/prebuilts/build-tools
   repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample
   repo_sync vendor/delphi/android_devices \
             vendor/delphi/bb_reprogramming \
             vendor/delphi/binaries_android_tunermanager \
             vendor/delphi/android_diagnostics \
             vendor/delphi/binaries_android_traffic

   # repo sync vendor/google/apps/GAS only needed since gms.mk is included by other makefiles called by lunch
   repo_sync vendor/google/apps/GAS
else
   cd "${REPO_ROOT_DIR}"
fi

set +x
source "$REPO_ROOT_DIR"/build/envsetup.sh
set -x
lunch ihu_vcc-eng
set +x
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"
set -x

if [ "$test_type" == "dryrun" ]; then
   # Fetch files uploaded to artinfcm (build for an open change in Gerrit)
   OUT_ARCHIVE=out.tgz
   rm -rf out "${OUT_ARCHIVE}"

   time scp -o StrictHostKeyChecking=no jenkins@artinfcm.volvocars.net:/home/jenkins/archive/*/"${ZUUL_COMMIT}"/"${OUT_ARCHIVE}" . \
       || die "Could not fetch archive from artinfcm."

   tar xf "${OUT_ARCHIVE}" || die "Could not extract out archive."
   rm "${OUT_ARCHIVE}"
fi

capability=""
if echo "${JOB_NAME}" | grep "test-flexray" >/dev/null; then
    capability="flexray flexray_or_carsim"
    export DATAELEMENTS_MODE=canoe
    export VECTOR_FDX_IP
    VECTOR_FDX_IP=$(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/get_flexray_IP.py)
    ping -c1 "${VECTOR_FDX_IP}"
    export power_cycle_length=5
    python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/flexray_ipm_reboot.py "$power_cycle_length"
elif [ "${JOB_NAME}" = "ihu_hourly_test-rig" ]; then
    capability="audio apix cem"
fi
export capability

export MP_PART_NUMBER
MP_PART_NUMBER=$(redis-cli get icup_android.jenkins."${UPSTREAM_JOB_JOBNAME}"."${UPSTREAM_JOB_NUMBER}".mp_part_number)

set +e

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
if [ "$test_type" == "dryrun" ]; then
   # out.tgz already fetched via scp command above
   time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
                     --plan hourly \
                     --report_results_to_ci_database \
                     --update_ihu \
                     -c ihu-generic adb mp-serial vip-serial ${capability} \
                     -o ${capability}
else
   time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan hourly \
    --vcc_dashboard_reporting \
    --report_results_to_ci_database \
    --update_ihu \
    --download "${UPSTREAM_JOB_JOBNAME}"/"${UPSTREAM_JOB_NUMBER}"_"${MP_PART_NUMBER}"/out.tgz \
    -c ihu-generic adb mp-serial vip-serial flexray_or_carsim ${capability} \
    -o ${capability}
fi
status=$?

set -e

echo "Logs and results are stored in db"

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi

