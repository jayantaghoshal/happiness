#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##
# Repo sync
#
bootstrap_docker_run "repo init -u ${UPSTREAM_JOB_GIT_URL} -b ${UPSTREAM_JOB_GIT_REVISION}" || die "repo init failed"
#fetching only the required codes for running the tests
SYNC_OPTIONS="--no-clone-bundle --force-sync --detach --optimized-fetch --current-branch -q -j4"
SYNC_REPOSITORIES="aosp/platform/build/soong \
aosp/platform/build/blueprint \
aosp/platform/prebuilts/go/linux-x86 \
aosp/platform/prebuilts/build-tools \
vendor/volvocars \
aosp/platform/build \
bsp/device/delphi/volvoihu \
aosp/platform/packages/services/Car \
aosp/device/sample \
vendor/delphi/android_devices \
vendor/delphi/bb_reprogramming \
vendor/delphi/binaries_android_tunermanager \
vendor/delphi/android_diagnostics \
vendor/google/apps/GAS"
bootstrap_docker_run "repo sync ${SYNC_OPTIONS} ${SYNC_REPOSITORIES}" || die "repo sync failed"
