#!/bin/bash
set -uex

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

"${SCRIPT_DIR}/hourly_test_common.sh"

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Update IHU
ihu_update || die "Failed to flash IHU image"

export VECTOR_FDX_IP=198.18.34.2
export VECTOR_FDX_PORT=2809

ping -c1 ${VECTOR_FDX_IP}

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan=hourly \
    -c ihu-generic adb mp-serial vip-serial flexray \
    -o flexray