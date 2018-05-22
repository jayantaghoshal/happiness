#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

export MP_PART_NUMBER
MP_PART_NUMBER=$(redis-cli get icup_android.jenkins."${UPSTREAM_JOB_JOBNAME}"."${UPSTREAM_JOB_NUMBER}".mp_part_number)

plan=""
download=""
if [ "${JOB_NAME}" = "ihu_daily_staging_test-generic" ] ||
   [ "${JOB_NAME}" = "ihu_daily_staging_test-flexray" ] ||
   [ "${JOB_NAME}" = "ihu_daily_staging_test-audio" ] ||
   [ "${JOB_NAME}" = "ihu_daily_staging_test-apix" ]
then
    plan="staging_daily"
    download="--download ${UPSTREAM_JOB_JOBNAME}/${UPSTREAM_JOB_NUMBER}_${MP_PART_NUMBER}/out.tgz"
elif [ "${JOB_NAME}" = "ihu_hourly_staging_test-generic" ] ||
     [ "${JOB_NAME}" = "ihu_hourly_staging_test-flexray" ] ||
     [ "${JOB_NAME}" = "ihu_hourly_staging_test-audio" ] ||
     [ "${JOB_NAME}" = "ihu_hourly_staging_test-apix" ]
then
    plan="staging_hourly"
    download="--download_latest ${UPSTREAM_JOB_JOBNAME}"
fi

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"

capability=""
if [ "${JOB_NAME}" = "ihu_daily_staging_test-flexray" ] ||
   [ "${JOB_NAME}" = "ihu_hourly_staging_test-flexray" ]
then
    capability="flexray flexray_or_carsim"
    export DATAELEMENTS_MODE=canoe
    export VECTOR_FDX_IP
    VECTOR_FDX_IP=$(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/get_flexray_IP.py)
    ping -c1 "${VECTOR_FDX_IP}"
elif [ "${JOB_NAME}" = "ihu_daily_staging_test-audio" ] ||
     [ "${JOB_NAME}" = "ihu_hourly_staging_test-audio" ]
then
    capability="audio"
elif [ "${JOB_NAME}" = "ihu_daily_staging_test-apix" ] ||
     [ "${JOB_NAME}" = "ihu_hourly_staging_test-apix" ]
then
    capability="apix"
fi

set +e

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan ${plan} \
    --vcc_dashboard_reporting \
    --report_results_to_ci_database \
    --update_ihu \
    ${download} \
    -c ihu-generic adb mp-serial vip-serial flexray_or_carsim ${capability} \
    -o ${capability}
status=$?

set -e

echo "Logs and results are stored in db"

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi