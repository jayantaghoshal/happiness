#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"


echo "Analyzing code"
python3 ./vendor/volvocars/tools/ci/shipit/bump.py . check "${ZUUL_BRANCH}"
docker_run "cd vendor/volvocars/tools/ci/jenkins && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && python3 -m unittest"
docker_run "cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh"