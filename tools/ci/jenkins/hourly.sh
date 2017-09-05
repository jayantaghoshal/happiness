#!/bin/bash
set -x

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Download from Artifactory
docker_run "artifactory pull ihu_image_build ${UPSTREAM_JOB_NUMBER} out.tgz" || die "artifactory pull failed"

# Unpack out.tgz
docker_run "tar xvfz out.tgz" || die "Unpack out.tgz failed"

# Update IHU
ihu_update || die "Failed to flash IHU image"

# Run tests
# TBD...

# Mark image as LSV if hourly passed
docker_run "redis-cli set icup_android.jenkins.ihu_image_build.lsv.job_number ${UPSTREAM_JOB_NUMBER}"


