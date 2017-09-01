#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

if [ $# -eq 0 ]; then
  echo "Usage: $0 <relative path to test directory>"
  exit 1
fi

TEST_DIR="$CURRENT_MODULE_DIR/$1"

echo -e "${C_NOTICE}Build vts framework...${C_OFF}"
docker_run "if ! type vts-tradefed >/dev/null 2>&1; then m vts -j4; fi" || exit 1

echo -e "${C_NOTICE}Build VTS gtest target runner...${C_OFF}"
pushd "$REPO_ROOT_DIR"/test/vts/runners/target/gtest > /dev/null || exit 1
docker_run "mm -j4" || exit 1
popd > /dev/null

echo -e "${C_NOTICE}Build module ${CURRENT_MODULE} in ${CURRENT_MODULE_DIR}...${C_OFF}"
pushd "$CURRENT_MODULE_DIR" > /dev/null || exit 1
docker_run "mm -j4" || exit 1
popd > /dev/null

echo -e "${C_NOTICE}Build test for ${CURRENT_MODULE} in ${TEST_DIR}...${C_OFF}"
pushd "$TEST_DIR" > /dev/null || exit 1
docker_run "mm -j4" || exit 1
popd > /dev/null

echo -e "${C_OK}Build OK!${C_OFF}"