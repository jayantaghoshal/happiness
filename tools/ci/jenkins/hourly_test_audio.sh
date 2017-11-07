#!/bin/bash
set -uex

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

"${SCRIPT_DIR}/hourly_test_common.sh"

# Run Unit and Component tests for vendor/volvocars
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run \
    --plan=hourly \
    -c ihu-generic adb mp-serial vip-serial audio \
    -o audio
