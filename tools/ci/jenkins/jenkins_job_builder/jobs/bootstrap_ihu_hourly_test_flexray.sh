#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##
# Repo sync
#
IHU_IMAGE_BUILD_COMMIT=$(bootstrap_docker_run "redis-cli get icup_android.jenkins.ihu_image_build.${UPSTREAM_JOB_NUMBER}.commit") || die "Failed to get commit from Redis"

if [[ ! -d .repo ]]; then
    bootstrap_docker_run "repo init -u ssh://gotsvl1415.got.volvocars.net:29421/manifest -b ${IHU_IMAGE_BUILD_COMMIT}" || die "repo init failed"
fi
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j8 vendor/volvocars" || die "repo sync failed"

##
# Run Tests
#
docker_killall
./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh ./vendor/volvocars/tools/ci/jenkins/hourly_test_flexray.sh