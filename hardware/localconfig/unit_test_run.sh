#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../..

# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
C_OK="\033[0;32m"
C_OFF="\033[0m"

pushd $SCRIPT_DIR > /dev/null

TEST_RESULT=1
TEST_OUTPUT_FILE=$(mktemp)

# Run unit test
$REPO_ROOT_DIR/vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && vts-tradefed run commandAndExit vts --skip-all-system-status-check --skip-preconditions --abi x86_64 --module VtsLocalConfigTestCases" | tee $TEST_OUTPUT_FILE

# Check that output file is not empty, or if there are any failed tests, or if output
# has zero passed tests, or if output doesn't contain PASSED: [1-9]
if [[ ! -s $TEST_OUTPUT_FILE ]] || grep -q -E "fail:|PASSED: 0" $TEST_OUTPUT_FILE || ! grep -q -E "PASSED: [1-9]" $TEST_OUTPUT_FILE; then
  echo -e "\n${C_ERROR}Test FAILED!${C_OFF}\n"
else
  echo -e "\n${C_OK}Test OK!${C_OFF}\n"
  TEST_RESULT=0
fi

# We can remove the output file. We use tee to the file, so the stdout is available.
rm -f $TEST_OUTPUT_FILE

popd > /dev/null

exit $TEST_RESULT
