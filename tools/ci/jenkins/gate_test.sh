#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

docker_killall

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

# Pull out files uploaded to Artifactory in gate_build.sh.
OUT_ARCHIVE=out.tgz
rm -rf out ${OUT_ARCHIVE}

docker_run "artifactory pull ihu_gate_build \"${ZUUL_CHANGE_IDS}\" ${OUT_ARCHIVE}" \
    || die "Could not pull out archive from Artifactory."

tar xvf ${OUT_ARCHIVE} || die "Could not extract out archive."
rm ${OUT_ARCHIVE}

ihu_update

# Run Unit and Component tests for vendor/volvocars
docker_run "time python3 $REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=gate -c ihu-generic adb mp-serial vip-serial
