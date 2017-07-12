#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=${SCRIPT_DIR}/../../../..

DOCKER_IMAGE_REFERENCE_FILE="${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/image.ref"
DOCKER_IMAGE=`cat ${DOCKER_IMAGE_REFERENCE_FILE}`

WORKING_DIR="$(pwd)"

VOLUMES=${VOLUMES:---volume $HOME:$HOME --volume $PWD:$PWD}

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

    # ccache must be initialized properly,
    # just creating the directory will not work, when building you will get Error: "ccache: failed to create  (No such file or directory)"
    # There is no "init" command but calling --max-size seems to do the trick
    ccache --max-size=5G
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
    $VOLUMES \
    --env=HOST_UID=$(id -u) \
    --env=HOST_GID=$(id -g) \
    --env=HOST_UNAME=$(id -un) \
    --env=CCACHE_DIR=${CCACHE_DIR} \
    --env=USE_CCACHE=${USE_CCACHE} \
    --env=REPO_ROOT_DIR=${REPO_ROOT_DIR} \
    --env=HOME=$HOME \
    --workdir ${WORKING_DIR} \
    --privileged \
    --volume /dev/bus/usb:/dev/bus/usb \
    ${DOCKER_IMAGE} \
    $*
