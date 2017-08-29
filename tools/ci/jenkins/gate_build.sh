#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Setup ccache
USE_CCACHE=true

docker_run "cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && python3 -m unittest"
docker_run "lunch ihu_vcc-eng && make -j16 droid"
