#!/bin/bash
set -ex

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Test IHU Update
"${SCRIPT_DIR}/hourly_test_common.sh"

# Test ADB
docker_run "adb shell getprop"

# Run tests
docker_run "time python3 $REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py run --plan=gate -c ihu-generic adb mp-serial vip-serial
