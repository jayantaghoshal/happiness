#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

#TODO: Move this to commit check when the commit gate is ready
#TODO: Investigate Hidl tool generator for multilib flag before
#uncommenting the following line
# 64bit_sanity.py "$REPO_ROOT_DIR"/vendor/volvocars/ || die "64 bit build sanity check failed"

(repo sync prebuilts/clang/host/linux-x86)
(repo sync prebuilts/build-tools)

(fix_cpp_format.sh vendor/volvocars --verify)
(fix_bp_format.sh vendor/volvocars --verify)

(cd vendor/volvocars && ./tools/ci/shell/static_analyze_cwd.sh)
(cd vendor/volvocars/tools/ci/shipit && ./analyze.sh)
(cd vendor/volvocars/tools/ci/shipit && python3 -m unittest)
(cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh)
(cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh)
(python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py analyze)
python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/cpp/logging_linter.py "$REPO_ROOT_DIR"/vendor/volvocars/
