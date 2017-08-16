#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

if [ $# -eq 0 ]; then
  echo "Usage: $0 <relative path to test directory>"
  exit 1
fi

TEST_DIR="$CURRENT_MODULE_DIR/$1"

echo -e "${C_NOTICE}Run test for ${CURRENT_MODULE} in ${TEST_DIR}...${C_OFF}"
docker_run "${SCRIPT_DIR}/../shipit/test_run.py ${TEST_DIR}"