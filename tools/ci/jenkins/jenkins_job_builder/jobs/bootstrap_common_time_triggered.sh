#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

die() { echo "$@" 1>&2 ; exit 1; }

##
# Configure Docker
#
export BOOTSTRAP_DOCKER_IMAGE=swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build:If943907d331a19834bdfea658f72144a0e503a08
export WORKSPACE_ROOT
WORKSPACE_ROOT=$(pwd)

bootstrap_docker_run () {
    docker run \
    --hostname aic-docker \
    --volume "$WORKSPACE_ROOT":"$WORKSPACE_ROOT" \
    --volume "$HOME":"$HOME" \
    --volume "$RAMDISK_ROOT":"$RAMDISK_ROOT" \
    --env=HOST_UID="$(id -u)" \
    --env=HOST_GID="$(id -g)" \
    --env=HOST_UNAME="$(id -un)" \
    --env=REPO_ROOT_DIR="${WORKSPACE_ROOT}" \
    --env=HOME="$HOME" \
    --env BASE_LOG_PATH \
    --env LOG_PATH \
    --env WORKSPACE_ROOT \
    --workdir "${WORKSPACE_ROOT}" \
    "${BOOTSTRAP_DOCKER_IMAGE}" \
    "$@"
}

# Make sure there are no other Docker containers left running on slaves that
# might interfer with current job. E.g. it is promplematic to have an adb server
# running in another container if we want to invoke it in the current job.
function docker_killall() {
  local containers
  containers=$(docker ps -q --format="{{.ID}} {{.Image}}" | grep vcc_aosp_build | cut -d " " -f 1 )
  if [ -n "$containers" ]; then
    #shellcheck disable=SC2086
    docker kill $containers
  fi
}
