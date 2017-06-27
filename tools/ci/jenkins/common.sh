#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f ${SCRIPT_DIR}/../../../../..)
ENTRYPOINT="${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/entrypoint.sh"
DOCKER_IMAGE_REFERENCE_FILE="${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/image.ref"
DOCKER_IMAGE=`cat ${DOCKER_IMAGE_REFERENCE_FILE}`
WORKING_DIR="$(pwd)"

if [ -z ${WORSPACE} ]; then 
  WORKSPACE=$(readlink -f ${REPO_ROOT_DIR}/..)
fi

# Setup ccache
if [ -z ${USE_CCACHE} ]; then 
  USE_CCACHE=0
fi
if [ -z ${HOST_CCACHE_DIR} ]; then 
  HOST_CCACHE_DIR=$(readlink -f ${REPO_ROOT_DIR}/../vcc-aosp-build-ccache)
fi

# Detect environment of docker command
INTERACTIVE_OPTS="-"

# Check if STDIN file is pipe. If not, it is "regular" STDIN
[[ -p /dev/fd/0 ]] || INTERACTIVE_OPTS="${INTERACTIVE_OPTS}i"

# Check if STDIN descriptor is associated with a terminal device
[[ -t 0 ]] && INTERACTIVE_OPTS="${INTERACTIVE_OPTS}t"
[[ "${INTERACTIVE_OPTS}" == "-" ]] && INTERACTIVE_OPTS=""

docker_run () {
    docker run \
    ${INTERACTIVE_OPTS} \
    --hostname aic-docker \
    --volume $WORKSPACE:$WORKSPACE \
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
    --entrypoint=${ENTRYPOINT} \
    ${DOCKER_IMAGE} \
    $*
}
