#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

# Update the manifests based on the templates for gate test
time python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local no_sync "${ZUUL_PROJECT}"

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
rm -rf out "${OUT_ARCHIVE}"

artifactory pull ihu_gate_build "${ZUUL_COMMIT}" "${OUT_ARCHIVE}" \
    || die "Could not pull out archive from Artifactory."

tar xvf "${OUT_ARCHIVE}" || die "Could not extract out archive."
rm "${OUT_ARCHIVE}"

set +e
for tmp in 1 2 3
do
    # Update IHU
    ihu_update
    if [ $? -eq 0 ]; then # On success, break
        break
    elif [ $tmp -eq 3 ]; then
        die "Failed to flash IHU image" # failed after three attempts
    else
        python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/jenkins/ihu_ipm_reboot.py # reboot ihu on flashing failures
    fi
done
set -e

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=gate --ci_reporting true -c ihu-generic adb mp-serial vip-serial
