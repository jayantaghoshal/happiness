#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"
set -x

capability=""
if [ "${JOB_NAME}" = "ihu_daily_test-flexray" ]
then
    capability="flexray flexray_or_carsim"
    export DATAELEMENTS_MODE=canoe
    export VECTOR_FDX_IP
    VECTOR_FDX_IP=$(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/get_flexray_IP.py)
    ping -c1 "${VECTOR_FDX_IP}"
elif [ "${JOB_NAME}" = "ihu_daily_test-rig" ]
then
    capability="audio apix cem"
fi
export capability

export MP_PART_NUMBER
MP_PART_NUMBER=$(redis-cli get icup_android.jenkins."${UPSTREAM_JOB_JOBNAME}"."${UPSTREAM_JOB_NUMBER}".mp_part_number)

set +e

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan nightly \
    --vcc_dashboard_reporting \
    --report_results_to_ci_database \
    --update_ihu \
    --download "${UPSTREAM_JOB_JOBNAME}"/"${UPSTREAM_JOB_NUMBER}"_"${MP_PART_NUMBER}"/out.tgz \
    -c ihu-generic adb mp-serial vip-serial flexray_or_carsim ${capability} \
    -o ${capability}
status=$?

set -e

echo "Logs and results are stored in db"

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi
