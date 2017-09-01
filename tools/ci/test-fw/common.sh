#!/bin/bash
# We ignore a lot of "unused variable" warnings in this file
# OK because this file is supposed to be "sourced"


SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../../..
DOCKER_BUILD_DIR=${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build
CURRENT_MODULE_DIR=$(pwd)
# shellcheck disable=SC2034
CURRENT_MODULE=$(basename "${CURRENT_MODULE_DIR}")
DOCKER_HOST_NAME="aic-docker"

# Color codes which can be used when printing stuff to terminal
# shellcheck disable=SC2034
C_ERROR="\033[0;31m"
# shellcheck disable=SC2034
C_OK="\033[0;32m"
# shellcheck disable=SC2034
C_NOTICE="\033[0;33m"
# shellcheck disable=SC2034
C_OFF="\033[0m"

# If we are not running inside docker container
# run command with docker run wrapper, otherwise
# just run command.
function docker_run {
  if [[ $HOSTNAME != "$DOCKER_HOST_NAME" ]]; then
    "${DOCKER_BUILD_DIR}"/run.sh "lunch ihu_vcc-eng && $*"
  else
    #shellcheck disable=SC2048
    #shellcheck disable=2086
    eval $*
  fi
}

# If running inside docker container, we need to issue lunch to setup environment
if [[ "$HOSTNAME" == "$DOCKER_HOST_NAME" ]]; then
  source "${REPO_ROOT_DIR}"/build/envsetup.sh
  lunch ihu_vcc-eng
fi