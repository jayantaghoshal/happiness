#!/bin/bash
set -uex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

docker_killall

# Sync repos required for build/envsetup.sh and lunch so we can run VTS.
repo_sync aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample

# Download from Artifactory
docker_run "artifactory pull ihu_image_build ${UPSTREAM_JOB_NUMBER} out.tgz" || die "artifactory pull failed"

# Unpack out.tgz
docker_run "tar xfz out.tgz" || die "Unpack out.tgz failed"

# Update IHU
ihu_update || die "Failed to flash IHU image"