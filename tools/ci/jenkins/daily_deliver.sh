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

# Download from Artifactory
artifactory pull "${UPSTREAM_JOB_JOBNAME}" "${UPSTREAM_JOB_NUMBER}"_"${MP_PART_NUMBER}" dist.zip || die "artifactory pull failed"

# unzip only the MP vbf file
unzip -j dist.zip "dist/mp-android*.VBF" -d "${REPO_ROOT_DIR}"/out

# unzip only the vip target files
unzip -j dist.zip "dist/ihu_vcc-target_files*.zip" -d "${REPO_ROOT_DIR}"/out

# unzip vbf - part1
unzip -j "./out/ihu_vcc-target_files*.zip" "VENDOR/vip-update/app/*.VBF" -d "${REPO_ROOT_DIR}"/out

# unzip vbf - part2
unzip -j "./out/ihu_vcc-target_files*.zip" "VENDOR/vip-update/pbl/*.VBF" -d "${REPO_ROOT_DIR}"/out

# rm the zip
rm -rf "${REPO_ROOT_DIR}"/out/*.zip

# change file name which is compatible for SWAT
python "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/change_vbf_file_name.py "${REPO_ROOT_DIR}"/out

# Publish VBF using SWAT
python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/publish_using_swat.py "${REPO_ROOT_DIR}"/out

rm -rf "${REPO_ROOT_DIR}"/out
