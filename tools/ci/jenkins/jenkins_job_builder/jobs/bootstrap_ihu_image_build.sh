#!/bin/bash

##
# Repo sync
#
if [[ ! -d .repo ]]; then
    docker_run "repo init -u ${GIT_URL} -b ${GIT_COMMIT}" || die "repo init failed"
fi
docker_run "repo sync --no-clone-bundle --current-branch -q -j8" || die "repo sync failed"

##
# Build image (clean build, sync all files and build without ccache)
#
./vendor/volvocars/tools/ci/jenkins/image_build.sh