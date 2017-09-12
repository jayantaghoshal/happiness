#!/bin/bash
##
# Repo sync
#
IHU_IMAGE_BUILD_COMMIT=$(docker_run "redis-cli get icup_android.jenkins.ihu_image_build.${UPSTREAM_JOB_NUMBER}.commit") || die "Failed to get commit from Redis"

if [[ ! -d .repo ]]; then
    docker_run "repo init -u ssh://gotsvl1415.got.volvocars.net:29421/manifest -b ${IHU_IMAGE_BUILD_COMMIT}" || die "repo init failed"
fi
docker_run "repo sync --no-clone-bundle --current-branch -q -j8 vendor/volvocars" || die "repo sync failed"

##
# Run Tests
#
./vendor/volvocars/tools/ci/jenkins/hourly.sh