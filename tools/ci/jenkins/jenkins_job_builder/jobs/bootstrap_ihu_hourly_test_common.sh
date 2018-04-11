#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##
# Repo sync
#
bootstrap_docker_run "repo init -u ${UPSTREAM_JOB_GIT_URL} -b ${UPSTREAM_JOB_GIT_REVISION}" || die "repo init failed"
#fetching only the required codes for running the tests
bootstrap_docker_run "repo sync --no-clone-bundle --force-sync --detach --current-branch -q -j4 aosp/platform/build/soong aosp/platform/build/blueprint aosp/platform/prebuilts/go/linux-x86 aosp/platform/prebuilts/build-tools" || die "repo sync failed"
bootstrap_docker_run "repo sync --no-clone-bundle --force-sync --detach --current-branch -q -j4 vendor/volvocars aosp/platform/build bsp/device/delphi/volvoihu aosp/platform/packages/services/Car aosp/device/sample" || die "repo sync failed"
bootstrap_docker_run "repo sync --no-clone-bundle --force-sync --detach --current-branch -q -j4 vendor/delphi/android_devices vendor/delphi/bb_reprogramming vendor/delphi/binaries_android_tunermanager vendor/delphi/android_diagnostics" || die "repo sync failed"
bootstrap_docker_run "repo sync --no-clone-bundle --force-sync --detach --current-branch -q -j4 vendor/google/apps/GAS" || die "repo sync failed"

##
# Run Tests
#
docker_killall
./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh ./vendor/volvocars/tools/ci/jenkins/hourly_test.sh
