#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../..

IMAGE_NAME=vcc_aosp_build

WORKING_DIR="$(pwd)"

# Setup ccache
if type ccache >/dev/null 2>&1; then CCACHE_EXISTS=1; else CCACHE_EXISTS=0; fi
USE_CCACHE=${USE_CCACHE:-${CCACHE_EXISTS}}
if [ "$USE_CCACHE" == "1" ]; then
  if [ -z $CCACHE_DIR ]; then
    if [ $CCACHE_EXISTS == "1" ]; then
      CCACHE_DIR=$(ccache --print-config | grep cache_dir\ = | awk '{ print $4 }')
    else
      CCACHE_DIR=$HOME/.ccache
    fi
  fi
  if ! test -e ${CCACHE_DIR}; then
    echo "Creating directory ${CCACHE_DIR}..."
    mkdir -p ${CCACHE_DIR}
  fi
fi

# Detect environment of docker command
INTERACTIVE_OPTS="-"
# Check if STDIN file is pipe. If not, it is "regular" STDIN
[[ -p /dev/fd/0 ]] || INTERACTIVE_OPTS="${INTERACTIVE_OPTS}i"
# Check if STDIN descriptor is associated with a terminal device
[[ -t 0 ]] && INTERACTIVE_OPTS="${INTERACTIVE_OPTS}t"
[[ "${INTERACTIVE_OPTS}" == "-" ]] && INTERACTIVE_OPTS=""

docker run \
    ${INTERACTIVE_OPTS} \
    --hostname aic-docker \
    --volume $HOME:$HOME  \
    --volume $PWD:$PWD    \
    --env=HOST_UID=$(id -u) \
    --env=HOST_GID=$(id -g) \
    --env=HOST_UNAME=$(id -un) \
    --env=CCACHE_DIR=${CCACHE_DIR} \
    --env=USE_CCACHE=${USE_CCACHE} \
    --env=REPO_ROOT_DIR=${REPO_ROOT_DIR} \
    --env=HOME=$HOME \
    -w ${WORKING_DIR} \
    --privileged \
    --volume /dev/bus/usb:/dev/bus/usb \
    --entrypoint="${SCRIPT_DIR}/entrypoint.sh" \
    ${IMAGE_NAME} \
    $*
