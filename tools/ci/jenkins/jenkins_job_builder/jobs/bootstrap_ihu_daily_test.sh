#!/bin/bash
##
# Repo sync
#
LATEST_IMAGE_BUILD=$(docker_run "redis-cli get icup_android.jenkins.ihu_image_build.latest.job_number") || die "Failed to get latest image_build job number from Redis"
IHU_IMAGE_BUILD_COMMIT=$(docker_run "redis-cli get icup_android.jenkins.ihu_image_build.${LATEST_IMAGE_BUILD}.commit") || die "Failed to get commit from Redis"


if [[ ! -d .repo ]]; then
    docker_run "repo init -u ssh://gotsvl1415.got.volvocars.net:29421/manifest -b ${IHU_IMAGE_BUILD_COMMIT}" || die "repo init failed"
fi
docker_run "repo sync --no-clone-bundle --current-branch -q -j8 vendor/volvocars" || die "repo sync failed"

##
# Run Tests
#
export UPSTREAM_JOB_NUMBER=${LATEST_IMAGE_BUILD}
./vendor/volvocars/tools/ci/jenkins/daily.sh