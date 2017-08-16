#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Pull out files uploaded to Artifactory in gate_build.sh.
OUT_ARCHIVE=out.tar.gz
rm -rf out ${OUT_ARCHIVE}

docker_run artifactory pull ihu_gate_build "${ZUUL_CHANGE_IDS}" ${OUT_ARCHIVE} \
    || die "Could not pull out archive from Artifactory."

tar xvf ${OUT_ARCHIVE} || dir "Could not extract out archive."
rm ${OUT_ARCHIVE}

# Dummy test case to test whole CI chain down to device
# ./out/host/linux-x86/bin/adb shell ls
