#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"

capability=""
if [ "${JOB_NAME}" = "ihu_hourly_test-flexray" ] || [ "${JOB_NAME}" = "ihu_staging_test-flexray" ] ; then
    capability="flexray"
    export VECTOR_FDX_IP
    VECTOR_FDX_IP=$(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/get_flexray_IP.py)
    ping -c1 "${VECTOR_FDX_IP}"
elif [ "${JOB_NAME}" = "ihu_hourly_test-audio" ]; then
    capability="audio"
elif [ "${JOB_NAME}" = "ihu_hourly_test-cem" ]; then
    capability="cem"
elif [ "${JOB_NAME}" = "ihu_hourly_test-apix" ]; then
    capability="apix"
fi
export capability

set +e

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan hourly \
    --vcc_dashboard_reporting \
    --report_results_to_ci_database \
    --update_ihu \
    --download ihu_hourly_build-eng/"${UPSTREAM_JOB_NUMBER}"/out.tgz \
    -c ihu-generic adb mp-serial vip-serial ${capability} \
    -o ${capability}
status=$?

set -e

echo "Logs and results are stored in db"

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi

