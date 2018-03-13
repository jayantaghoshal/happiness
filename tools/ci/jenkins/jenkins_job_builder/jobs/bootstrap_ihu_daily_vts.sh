#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##
# Repo sync
#
bootstrap_docker_run "repo init -u ${UPSTREAM_JOB_GIT_URL} -b ${UPSTREAM_JOB_GIT_REVISION}" || die "repo init failed"
#fetching only the required codes for running the tests
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j32 vendor/volvocars aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample" || die "repo sync failed"
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j32 vendor/delphi/android_device_release vendor/delphi/android_device" || die "repo sync failed"
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j32 vendor/delphi/android_vendor vendor/delphi/bb_reprogramming vendor/delphi/binaries_android_tunermngr vendor/delphi/android_diagnostics" || die "repo sync failed"

##
# Run Tests
#
docker_killall
./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh ./vendor/volvocars/tools/ci/jenkins/vts_test.sh
