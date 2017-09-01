#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

# Pull out files uploaded to Artifactory in gate_build.sh.
OUT_ARCHIVE=out.tar.gz
rm -rf out ${OUT_ARCHIVE}

docker_run artifactory pull ihu_gate_build "${ZUUL_CHANGE_IDS}" ${OUT_ARCHIVE} \
    || die "Could not pull out archive from Artifactory."

tar xvf ${OUT_ARCHIVE} || die "Could not extract out archive."
rm ${OUT_ARCHIVE}

ihu_update

# Dummy test case to test whole CI chain down to device
# ./out/host/linux-x86/bin/adb shell ls
