#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
DOCKER_BUILD_DIR=${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build

# Setup ccache
if [ -z "${USE_CCACHE:-}" ]; then
  export USE_CCACHE=false
fi
if [ -z "${CCACHE_DIR:-}" ]; then
  CCACHE_DIR=$(readlink -f "${REPO_ROOT_DIR}"/../vcc-aosp-build-ccache)
  export CCACHE_DIR
fi


function docker_run() {
  local volumes
  local mp_dev=/dev/ttyMP
  local vip_dev=/dev/ttyVIP

  volumes=--volume="${WORKSPACE}":"${WORKSPACE}"\ --volume="${CCACHE_DIR}":"${CCACHE_DIR}"\ --volume=$mp_dev:$mp_dev\ --volume=$vip_dev:$vip_dev

  # Need to create dirs passed as volumes that may not exist so Docker does not
  # create them with root.root ownership.
  mkdir -p "${CCACHE_DIR}"

  if [ -d "/mnt/ramdisk" ]; then
    volumes=${volumes}\ --volume=/mnt/ramdisk:/mnt/ramdisk
  fi

  #shellcheck disable=SC2048
  #shellcheck disable=2086
  "${DOCKER_BUILD_DIR}"/run.sh \
    --env-file="${SCRIPT_DIR}"/env.list \
    ${volumes} \
    $*
}

docker_run "$@"
