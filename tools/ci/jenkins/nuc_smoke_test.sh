#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

cd "${REPO_ROOT_DIR}"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"

export MP_PART_NUMBER
MP_PART_NUMBER=$(redis-cli get icup_android.jenkins."${UPSTREAM_JOB_JOBNAME}"."${UPSTREAM_JOB_NUMBER}".mp_part_number)

# Download from Artifactory
artifactory pull "${UPSTREAM_JOB_JOBNAME}" "${UPSTREAM_JOB_NUMBER}"_"${MP_PART_NUMBER}" out.tgz || die "artifactory pull failed"

# Unpack out.tgz
tar xfz out.tgz || die "Unpack out.tgz failed"

set +e
# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan gate --update_ihu -c ihu-generic adb mp-serial vip-serial
status=$?
set -e

# Check status
if [ $status -ne 0 ]; then
    die "Test failed"
fi