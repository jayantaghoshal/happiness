#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

docker_run "lunch ihu_vcc-eng && make -j16 flashfiles"
