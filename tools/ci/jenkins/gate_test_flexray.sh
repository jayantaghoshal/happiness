#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

docker_killall

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

# Pull out files uploaded to Artifactory in gate_build.sh.
OUT_ARCHIVE=out.tgz
rm -rf out ${OUT_ARCHIVE}

artifactory pull ihu_gate_build "${ZUUL_COMMIT}" "${OUT_ARCHIVE}" \
    || die "Could not pull out archive from Artifactory."

tar xvf ${OUT_ARCHIVE} || die "Could not extract out archive."
rm ${OUT_ARCHIVE}

ihu_update


export VECTOR_FDX_IP=198.18.34.2
export VECTOR_FDX_PORT=2809
ping -c1 ${VECTOR_FDX_IP}

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=gate -c ihu-generic adb mp-serial vip-serial flexray -o flexray
