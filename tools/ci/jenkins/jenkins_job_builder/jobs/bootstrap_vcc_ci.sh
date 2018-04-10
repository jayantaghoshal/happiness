#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

export BOOTSTRAP_DOCKER_IMAGE=swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build:If943907d331a19834bdfea658f72144a0e503a08
export WORKSPACE_ROOT
WORKSPACE_ROOT=$(pwd)
BOOTSTRAP_VOLUMES="--volume $WORKSPACE_ROOT:$WORKSPACE_ROOT --volume $HOME:$HOME"

bootstrap_docker_run () {
    #shellcheck disable=SC2086
    docker run \
        --hostname aic-docker \
        --dns=10.244.0.15 \
        --dns=10.244.0.20 \
        --dns-search=dhcp.nordic.volvocars.net \
        ${BOOTSTRAP_VOLUMES} \
        --env=HOST_UID="$(id -u)" \
        --env=HOST_GID="$(id -g)" \
        --env=HOST_UNAME="$(id -un)" \
        --env=REPO_ROOT_DIR="${WORKSPACE_ROOT}" \
        --env=HOME="$HOME" \
        --env BASE_LOG_PATH \
        --env LOG_PATH \
        --env WORKSPACE_ROOT \
        --workdir "${WORKSPACE_ROOT}" \
        --rm \
        "${BOOTSTRAP_DOCKER_IMAGE}" \
        "$@"
}

API_URL="http://cimb.volvocars.biz/api/1.6.0"

ihu_hourly_activity_started() {
    CHAIN_ID=$(bootstrap_docker_run redis-cli get icup_android.gerrit.commit_id."${GIT_COMMIT}".change_id)
    URL="https://icup_android.jenkins.cm.volvocars.biz/job/ihu_hourly/${BUILD_NUMBER}"
    LOG="https://icup_android.jenkins.cm.volvocars.biz/job/ihu_hourly/${BUILD_NUMBER}/consoleFull"
    curl "${API_URL}/ActivityStarted" \
        -d chain_id="${CHAIN_ID}" \
        -d name="ihu_hourly" \
        -d url="${URL}" \
        -d logs[]="${LOG}" \
        -d target="VCC-CI"
}

ihu_daily_activity_started() {
    CHAIN_ID=$(bootstrap_docker_run redis-cli get icup_android.gerrit.commit_id."${GIT_COMMIT}".change_id)
    URL="https://icup_android.jenkins.cm.volvocars.biz/job/ihu_daily/${BUILD_NUMBER}"
    LOG="https://icup_android.jenkins.cm.volvocars.biz/job/ihu_daily/${BUILD_NUMBER}/consoleFull"
    curl "${API_URL}/ActivityStarted" \
        -d chain_id="${CHAIN_ID}" \
        -d name="ihu_daily" \
        -d url="${URL}" \
        -d logs[]="${LOG}" \
        -d target="VCC-CI"
}

if [ "${JOB_NAME}" == "ihu_hourly" ]; then
    ihu_hourly_activity_started
fi

if [ "${JOB_NAME}" == "ihu_daily" ]; then
    ihu_daily_activity_started
fi
