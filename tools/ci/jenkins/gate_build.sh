#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Setup ccache
USE_CCACHE=1

docker_run "lunch ihu_vcc-eng && make -j16 droid"
