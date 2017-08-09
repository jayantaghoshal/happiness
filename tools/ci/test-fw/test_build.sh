#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../../..
CURRENT_MODULE_DIR=$(pwd)
CURRENT_MODULE=$(basename ${CURRENT_MODULE_DIR})

# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
C_OK="\033[0;32m"
C_OFF="\033[0m"

function run
{
  COMMAND=$1
  if [[ $USER != ihu ]]; then
    # If we are not running inside docker container
    # execute command with docker wrapper.
    $REPO_ROOT_DIR/vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && ${COMMAND}"
  else
    eval $COMMAND
  fi
}

if [ $# -eq 0 ]; then
  echo "Usage: $0 <relative path to test directory>"
  exit 1
fi

TEST_DIR="$CURRENT_MODULE_DIR/$1"

# If running inside docker container, we need to issue lunch to get the build commands used below.
if [[ $USER == ihu ]]; then
  source ${REPO_ROOT_DIR}/build/envsetup.sh
  lunch ihu_vcc-eng
fi

echo "Build vts framework..."
run "if ! type vts-tradefed >/dev/null 2>&1; then m vts -j4; fi" || exit 1

echo "Build VTS gtest target runner..."
pushd $REPO_ROOT_DIR/test/vts/runners/target/gtest > /dev/null || exit 1
run "mm -j4" || exit 1
popd > /dev/null

echo "Build module ${CURRENT_MODULE} in ${CURRENT_MODULE_DIR}..."
pushd $CURRENT_MODULE_DIR > /dev/null || exit 1
run "mm -j4" || exit 1
popd > /dev/null

echo "Build test for ${CURRENT_MODULE} in ${TEST_DIR}..."
pushd $TEST_DIR > /dev/null || exit 1
run "mm -j4" || exit 1
popd > /dev/null
