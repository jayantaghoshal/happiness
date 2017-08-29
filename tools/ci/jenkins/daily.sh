#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Download from Artifactory
docker_run "artifactory pull ihu_image_build ${UPSTREAM_JOB_NUMBER} out.tgz" || die "artifactory pull failed"

# Unpack out.tgz
docker_run "tar xvfz out.tgz" || die "Unpack out.tgz failed"

# Run tests
# TBD...

