#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

#TODO: Move this to commit check when the commit gate is ready
#TODO: Investigate Hidl tool generator for multilib flag before
#uncommenting the following line
# docker_run "64bit_sanity.py $REPO_ROOT_DIR/vendor/volvocars/" || die "64 bit build sanity check failed"

docker_run "cd vendor/volvocars && ./tools/ci/shell/static_analyze_cwd.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && python3 -m unittest"
docker_run "cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh"
docker_run "python3 $REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py analyze
docker_run "python3 $REPO_ROOT_DIR"/vendor/volvocars/tools/ci/cpp/logging_linter.py "$REPO_ROOT_DIR"/vendor/volvocars/
