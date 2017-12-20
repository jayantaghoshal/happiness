#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##
# Repo sync
#
bootstrap_docker_run "repo init -u ${GIT_URL} -b ${GIT_COMMIT}" || die "repo init failed"
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j32" || die "repo sync failed"

##
# Build image (clean build, sync all files and build without ccache)
#
./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh ./vendor/volvocars/tools/ci/jenkins/image_build.sh
