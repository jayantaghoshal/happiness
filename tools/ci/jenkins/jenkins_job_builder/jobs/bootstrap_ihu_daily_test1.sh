#!/bin/bash

##
# Repo sync
#
bootstrap_docker_run "repo init -u ${UPSTREAM_JOB_GIT_URL} -b ${UPSTREAM_JOB_GIT_REVISION}" || die "repo init failed"
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j8" || die "repo sync failed"
##
# Run Tests
#
docker_killall
./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh ./vendor/volvocars/tools/ci/jenkins/daily_test.sh
