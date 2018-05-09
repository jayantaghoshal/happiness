#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

#TODO: Move this to commit check when the commit gate is ready
#TODO: Investigate Hidl tool generator for multilib flag before
#uncommenting the following line
# 64bit_sanity.py "$REPO_ROOT_DIR"/vendor/volvocars/ || die "64 bit build sanity check failed"

time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/bump.py . local no_sync "${ZUUL_PROJECT}"

time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/bump.py . sync_repo_based_on_manifest aosp/platform/prebuilts/clang/host/linux-x86
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/bump.py . sync_repo_based_on_manifest aosp/platform/prebuilts/build-tools
#(repo sync --force-sync --detach prebuilts/clang/host/linux-x86)
#(repo sync --force-sync --detach prebuilts/build-tools)

commit-check.sh verify vendor/volvocars
licensing-components.py software-bom vendor/volvocars/ .

(cd vendor/volvocars/tools/ci/shipit && ./analyze.sh)
(cd vendor/volvocars/tools/ci/shipit && python3 -m unittest)
(cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh)
(cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh)

