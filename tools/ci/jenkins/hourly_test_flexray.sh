#!/bin/bash
set -uex

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

./hourly_test_common.sh


export VECTOR_FDX_IP
VECTOR_FDX_IP=198.18.34.2




ping -c1 ${VECTOR_FDX_IP}
echo "TODO: Start flexray tests"