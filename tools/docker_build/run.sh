#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../..

IMAGE_NAME=vcc_aosp_build
WORKING_DIR="$(pwd)"

# Setup ccache
USE_CCACHE=1
HOST_CCACHE_DIR=$HOME/vcc-aosp-build-docker-ccache
if ! test -e ${HOST_CCACHE_DIR}; then
  echo "Creating directory ${HOST_CCACHE_DIR}..."
  mkdir -p ${HOST_CCACHE_DIR}
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
    --env=CCACHE_DIR=${HOST_CCACHE_DIR} \
    --env=USE_CCACHE=${USE_CCACHE} \
    --env=REPO_ROOT_DIR=${REPO_ROOT_DIR} \
    --env=HOME=$HOME \
    -w ${WORKING_DIR} \
    --privileged \
    --volume /dev/bus/usb:/dev/bus/usb \
    ${IMAGE_NAME} \
    $*
