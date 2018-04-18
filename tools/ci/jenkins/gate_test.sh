#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

# Update the manifests based on the templates for gate test
time python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local no_sync "${ZUUL_PROJECT}"

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build/soong aosp/platform/build/blueprint aosp/platform/prebuilts/go/linux-x86 aosp/platform/prebuilts/build-tools
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample
repo_sync vendor/delphi/android_devices vendor/delphi/bb_reprogramming vendor/delphi/binaries_android_tunermanager vendor/delphi/android_diagnostics
# repo sync vendor/google/apps/GAS only needed since gms.mk is included by other makefiles called by lunch
repo_sync vendor/google/apps/GAS

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "$REPO_ROOT_DIR"/vendor/volvocars/tools/envsetup.sh

# Pull out files uploaded to Artifactory in gate_build.sh.
OUT_ARCHIVE=out.tgz
rm -rf out "${OUT_ARCHIVE}"

artifactory pull ihu_gate_build "${ZUUL_COMMIT}" "${OUT_ARCHIVE}" \
    || die "Could not pull out archive from Artifactory."

tar xvf "${OUT_ARCHIVE}" || die "Could not extract out archive."
rm "${OUT_ARCHIVE}"

capability=""
if [ "${JOB_NAME}" = "ihu_gate_test_flexray" ]
then
    capability="flexray"
    export VECTOR_FDX_IP
    VECTOR_FDX_IP=$(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/get_flexray_IP.py)
    ping -c1 "${VECTOR_FDX_IP}"
elif [ "${JOB_NAME}" = "ihu_gate_test_audio" ]
then
    capability="audio"
elif [ "${JOB_NAME}" = "ihu_gate_test_apix" ]
then
    capability="apix"
fi
export capability

ci_reporting=""
if [ "${JOB_NAME}" != "ihu_check_test" ]
then
    ci_reporting="--ci_reporting"
fi
export ci_reporting

# Run Unit and Component tests for vendor/volvocars
#shellcheck disable=SC2086
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan=gate \
    ${ci_reporting} \
    --update_ihu \
    --abort-on-first-failure \
    -c ihu-generic adb mp-serial vip-serial ${capability} \
    -o ${capability}
