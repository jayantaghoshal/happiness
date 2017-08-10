#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../..

DOCKER_IMAGE_REFERENCE_FILE="${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/image.ref"
DOCKER_IMAGE=`cat ${DOCKER_IMAGE_REFERENCE_FILE}`

WORKING_DIR="$(pwd)"

VOLUMES=${VOLUMES:---volume $HOME:$HOME --volume $PWD:$PWD}

# Handle options
for i in "$@"
do
case $i in
    --local)
    DOCKER_IMAGE="vcc_aosp_build"
    shift # past argument with no value
    ;;
    *)
            # unknown option
    ;;
esac
done

# Default to using ccache
# Technically unnecessary (the build system only checks for "not false") but explicit for clarity
if [[ ! -v USE_CCACHE ]]; then
  export USE_CCACHE=true
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
    $VOLUMES \
    --dns=10.244.0.15 \
    --dns=10.244.0.20 \
    --dns-search=dhcp.nordic.volvocars.net \
    --env=HOST_UID=$(id -u) \
    --env=HOST_GID=$(id -g) \
    --env=HOST_UNAME=$(id -un) \
    --env CCACHE_DIR \
    --env USE_CCACHE \
    --env=REPO_ROOT_DIR=${REPO_ROOT_DIR} \
    --env=HOME=$HOME \
    --workdir ${WORKING_DIR} \
    --privileged \
    --volume /dev/bus/usb:/dev/bus/usb \
    ${DOCKER_IMAGE} \
    $*
