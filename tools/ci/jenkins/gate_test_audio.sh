#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample
repo_sync vendor/delphi/android_device_release vendor/delphi/android_device
repo_sync vendor/delphi/android_vendor vendor/delphi/bb_reprogramming vendor/delphi/binaries_android_tunermngr vendor/delphi/android_diagnostics
# repo sync vendor/google/apps/GAS only needed since gms.mk is included by other makefiles called by lunch
repo_sync vendor/google/apps/GAS

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng
source "$REPO_ROOT_DIR"/vendor/volvocars/tools/envsetup.sh

# Pull out files uploaded to Artifactory in gate_build.sh.
OUT_ARCHIVE=out.tgz
rm -rf out ${OUT_ARCHIVE}

artifactory pull ihu_gate_build "${ZUUL_COMMIT}" "${OUT_ARCHIVE}" \
    || die "Could not pull out archive from Artifactory."

tar xvf ${OUT_ARCHIVE} || die "Could not extract out archive."
rm ${OUT_ARCHIVE}

ihu_update

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan=gate \
    -c ihu-generic adb mp-serial vip-serial audio \
    -o audio
