#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

##
# Repo sync
#
function get_repos {
    bootstrap_docker_run "repo init --reference=/cm/repo-mirror -u ${UPSTREAM_JOB_GIT_URL} -b ${UPSTREAM_JOB_GIT_REVISION}"
    if [ $? != 0 ]; then
        echo "repo init failed"
        return 1
    fi
    #fetching only the required codes for running the tests
    SYNC_OPTIONS="--no-clone-bundle --force-sync --detach --optimized-fetch --current-branch -q -j4"
    SYNC_REPOSITORIES="aosp/platform/build/soong \
    aosp/platform/build/blueprint \
    aosp/platform/prebuilts/go/linux-x86 \
    aosp/platform/prebuilts/build-tools \
    vendor/volvocars \
    vendor/volvocars/hmi/apps/tunerbrowserservice \
    vendor/volvocars/hmi/Launcher \
    aosp/platform/build \
    bsp/device/delphi/volvoihu \
    aosp/platform/packages/services/Car \
    aosp/device/sample \
    vendor/delphi/android_devices \
    vendor/delphi/bb_reprogramming \
    vendor/delphi/binaries_android_tunermanager \
    vendor/delphi/android_diagnostics \
    vendor/delphi/binaries_android_traffic \
    vendor/google/apps/GAS"
    bootstrap_docker_run "repo sync ${SYNC_OPTIONS} ${SYNC_REPOSITORIES}"
    if [ $? != 0 ]; then
        echo "repo sync failed"
        return 1
    fi
}
function clean_all {
    find . -type f -delete && find . -type d -empty -delete
}
get_repos || (clean_all && get_repos)
