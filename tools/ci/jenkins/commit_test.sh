#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f ${SCRIPT_DIR}/../../../../..)
ENTRYPOINT="${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/entrypoint.sh"
IMAGE_NAME="swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build:7920cefbdbb6f70761642b421b31fd6dc29ad84c
"
WORKING_DIR="$(pwd)"

if [ -z ${WORSPACE} ]; then 
  WORKSPACE=$(readlink -f ${REPO_ROOT_DIR}/..)
fi

# Setup ccache
USE_CCACHE=0
HOST_CCACHE_DIR=$(readlink -f ${REPO_ROOT_DIR}/../vcc-aosp-build-ccache)

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
    --entrypoint=${ENTRYPOINT} \
    ${IMAGE_NAME} \
    $*
}

docker_run "lunch ihu_abl_car-eng && make -j32 droid"
