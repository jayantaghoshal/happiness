#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

plan=""
if [ "${JOB_NAME}" = "ihu_staging_test-generic" ] ||
   [ "${JOB_NAME}" = "ihu_staging_test-flexray" ] ||
   [ "${JOB_NAME}" = "ihu_staging_test-audio" ] ||
   [ "${JOB_NAME}" = "ihu_staging_test-apix" ]
then
    plan="staging"
elif [ "${JOB_NAME}" = "ihu_incubator_test-generic" ] ||
     [ "${JOB_NAME}" = "ihu_incubator_test-flexray" ] ||
     [ "${JOB_NAME}" = "ihu_incubator_test-audio" ] ||
     [ "${JOB_NAME}" = "ihu_incubator_test-apix" ]
then
    plan="incubator"
fi

# Download from Artifactory
if [ "${plan}" = "staging" ]
then
    artifactory pull "${UPSTREAM_JOB_JOBNAME}" "${UPSTREAM_JOB_NUMBER}" out.tgz || die "artifactory pull failed"
elif [ "${plan}" = "incubator" ]
then
    artifactory pull-latest "${UPSTREAM_JOB_JOBNAME}" out.tgz || die "artifactory pull failed"
fi

# Unpack out.tgz
tar xfz out.tgz || die "Unpack out.tgz failed"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"

capability=""
if [ "${JOB_NAME}" = "ihu_staging_test-flexray" ] ||
   [ "${JOB_NAME}" = "ihu_incubator_test-flexray" ]
then
    capability="flexray"
    export VECTOR_FDX_IP
    VECTOR_FDX_IP=$(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/get_flexray_IP.py)
    ping -c1 "${VECTOR_FDX_IP}"
elif [ "${JOB_NAME}" = "ihu_staging_test-audio" ] ||
     [ "${JOB_NAME}" = "ihu_incubator_test-audio" ]
then
    capability="audio"
elif [ "${JOB_NAME}" = "ihu_staging_test-apix" ] ||
     [ "${JOB_NAME}" = "ihu_incubator_test-apix" ]
then
    capability="apix"
fi

set +e

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan ${plan} --ci_reporting --update_ihu -c ihu-generic adb mp-serial vip-serial ${capability} -o ${capability}
status=$?

set -e

echo "Logs and results are stored in db"

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi