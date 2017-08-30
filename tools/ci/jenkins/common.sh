#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f ${SCRIPT_DIR}/../../../../..)
DOCKER_BUILD_DIR=${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build

if [ -z ${WORSPACE} ]; then
  WORKSPACE=$(readlink -f ${REPO_ROOT_DIR}/..)
fi

# Setup ccache
if [ -z ${USE_CCACHE} ]; then
  USE_CCACHE=false
fi
if [ -z ${CCACHE_DIR} ]; then
  CCACHE_DIR=$(readlink -f ${REPO_ROOT_DIR}/../vcc-aosp-build-ccache)
fi

# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
C_OK="\033[0;32m"
C_OFF="\033[0m"

function die() {
    message=$1
    echo -e "${C_ERROR}${message}${C_OFF}"
    exit 1
}

function docker_run() {
  USE_CCACHE=$USE_CCACHE \
  CCACHE_DIR=$CCACHE_DIR \
  ${DOCKER_BUILD_DIR}/run.sh \
  --env-file=${SCRIPT_DIR}/env.list \
  --volume=$WORKSPACE:$WORKSPACE \
  $*
}
