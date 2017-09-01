#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}/../../../../..")

# This script will create a VTS test package to enable
# build on one machine and test run an another machine.
#
# On the build machine you need to have built the VTS
# framework using
#   $ make vts
#
# Optionally you may want to build other tests which
# are using the VTS framework, for example
#   $ vendor/volvocars/hardware/localconfig/
#
# Now you can run this script to create a test package. Transfer
# this package to another machine where the test should be executed.
# Unpack the package in AOSP repo root and run your tests.

# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
C_OK="\033[0;32m"
C_OFF="\033[0m"

function die {
  message=$1
  echo -e "${C_ERROR}Package creation failed - $message ${C_OFF}"
  exit 1
}

pushd "$REPO_ROOT_DIR" > /dev/null

TEST_PACKAGE_FILE=test_package.tar.gz

tar cvzf "$TEST_PACKAGE_FILE" \
  out/host/linux-x86/vts/android-vts \
  out/host/linux-x86/bin/adb \
  out/host/linux-x86/bin/vts-tradefed || die "Couldn't create package"

echo -e "${C_OK}VTS test package created ${REPO_ROOT_DIR}/${TEST_PACKAGE_FILE}. Please unpack in it AOSP REPO root to execute tests.${C_OFF}"

popd > /dev/null
