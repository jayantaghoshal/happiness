#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../..

# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
C_OK="\033[0;32m"
C_OFF="\033[0m"

pushd $SCRIPT_DIR > /dev/null

source $REPO_ROOT_DIR/build/envsetup.sh
lunch ihu_vcc-eng

# Build module
$REPO_ROOT_DIR/vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && mm -j4" || exit 1

# Build vts framework
if ! type vts-tradefed >/dev/null 2>&1; then
  $REPO_ROOT_DIR/vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && m vts -j 4" || exit 1
fi

# Build unit test
pushd test/ut > /dev/null
$REPO_ROOT_DIR/vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && mm -j4" || exit 1
popd > /dev/null


# Run unit test
TEST_OUTPUT=$(mktemp)

vts-tradefed run commandAndExit vts --skip-all-system-status-check --skip-preconditions --abi x86_64 --module VtsLocalConfigTestCases | tee $TEST_OUTPUT
if grep -q "fail:" $TEST_OUTPUT; then
  echo -e "\n${C_ERROR}Test FAILED!${C_OFF}\n"
else
  echo -e "\n${C_OK}Test OK!${C_OFF}\n"
fi

rm -f $TEST_OUTPUT

popd > /dev/null
