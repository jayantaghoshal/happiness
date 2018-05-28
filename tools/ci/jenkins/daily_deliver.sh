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

# install latest SWAT with sudo so it would keep the swat binary in the path
export PIP_CONFIG_FILE=""
sudo pip install SWAT --index-url https://"${JENKINS_USER}":"${JENKINS_API_PASSWORD}"@ci2.artifactory.cm.volvocars.biz/artifactory/api/pypi/PyPi/simple

rm -rf ~/SWAT
mkdir -p ~/SWAT
echo "[SERVER]" > ~/SWAT/SWAT_CONFIG.ini
echo "server_address = gotsvw11045.got.volvocars.net:8003" >> ~/SWAT/SWAT_CONFIG.ini

# change file name which is compatible for SWAT
python "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/change_vbf_file_name.py "${REPO_ROOT_DIR}"/out

# Publish VBF using SWAT
python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/publish_using_swat.py "${REPO_ROOT_DIR}"/out

rm -rf "${REPO_ROOT_DIR}"/out
