#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

WORKING_DIR="$(pwd)"
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}/../../../..")

DOCKER_IMAGE_REFERENCE_FILE="${SCRIPT_DIR}"/image.ref
DOCKER_IMAGE=$(cat "${DOCKER_IMAGE_REFERENCE_FILE}")

IS_TERMINAL=false
# Check if STDOUT descriptor (1) is associated with a terminal device
if [[ -t 1 ]]; then
    IS_TERMINAL=true
fi
IS_INTERACTIVE=false
# Check if STDIN descriptior is interactive.
if [[ -t 0 ]]; then
    IS_INTERACTIVE=true
fi

# Handle options
ENV_FILE_OPT=
VOLUMES="--volume=$HOME:$HOME --volume=$PWD:$PWD --volume=/dev/bus/usb:/dev/bus/usb"

for i in "$@"
do
case $i in
    --env-file=*)
    ENV_FILE_OPT=$i
    shift
    ;;
    --multiuser)
    MULTIUSER="true"
    shift # past argument with no value
    ;;
    --local)
    DOCKER_IMAGE="vcc_aosp_build"
    shift # past argument with no value
    ;;
    --volume=*)
    VOLUMES="$VOLUMES $i"
    shift
    ;;
    *)
            # unknown option
    ;;
esac
done


# Containers can be left dangling if you close the terminal running a contianer, then
# docker will not understand that the attached STDIN/OUT is dead and continue running.
# Might be possible to solve with different INTERACTIVE_OPS or --attach options

C_ERROR="\033[0;31m"
C_OFF="\033[0m"
#(curly braces as literals ok because they are templates used by docker ps)
#shellcheck disable=SC1083
EXISTING_CONTAINERS=$(docker ps --filter "ancestor=$DOCKER_IMAGE" --format={{.ID}})
EXISTING_CONTAINER_ARRAY=( $EXISTING_CONTAINERS )
if [[ !  -z  $EXISTING_CONTAINERS && "${MULTIUSER}" != "true" ]]; then
    echo -e "${C_ERROR}-------------------------------------------------------------------------------------------"
    echo "                              WARNING"
    echo ""
    echo "WARNING: You already have a IHU docker container running"
    echo "  This can interfere with hardware devices in the container such as ADB"
    echo ""
    echo "  Already running containers:"
    for c in "${EXISTING_CONTAINER_ARRAY[@]}"
    do
        echo -n "     $c  owner: "
        docker inspect "$c" --format '{{.Config.Env}}' | perl -pe 's,.*HOST_UNAME=(.*?)\s.*,\1,'
    done
    echo ""
    echo "  To view your running docker containers:"
    echo "       docker ps"
    echo
    echo "  To kill the container in case it is running in the background:"
    echo "       docker kill CONTAINER_ID"
    echo ""
    echo "  To attach a shell to the already running container:"
    echo "       docker exec --tty --interactive CONTAINER_ID /tmp/entrypoint.sh bash"
    echo "-------------------------------------------------------------------------------------------"


    if [[ $IS_TERMINAL == true && $IS_INTERACTIVE == true ]]; then
        echo ""
        echo ">>> Interactive mode -- You will now be auto attached to the already running container <<<"
        echo ""
        echo -e "${C_OFF}"
        docker exec --tty --interactive "${EXISTING_CONTAINER_ARRAY[0]}" /tmp/entrypoint.sh bash
        exit 0
    fi
    echo -e "${C_OFF}"
fi



# Default to using ccache
# Technically unnecessary (the build system only checks for "not false") but explicit for clarity
if [[ ! -v USE_CCACHE ]]; then
  export USE_CCACHE=true
fi

# Get a underscore-separated list of group ids for the host user groups
HOST_USER_GROUPS=$(id -G "$USER")
HOST_USER_GROUPS=${HOST_USER_GROUPS// /_}


INTERACTIVE_OPTS=""
if [[ $IS_INTERACTIVE == true ]]; then
    INTERACTIVE_OPTS="${INTERACTIVE_OPTS} --interactive "
fi
if [[ $IS_TERMINAL == true ]]; then
    INTERACTIVE_OPTS="${INTERACTIVE_OPTS} --tty "
fi

if [[ $IS_TERMINAL == true && $IS_INTERACTIVE == true ]]; then
    # Patch library.go if we want to skip abi checks
    if [ ! -z "${SKIP_ABI_CHECKS+x}" ]; then
        # Patch build/soong/cc/library.go to skip generating .sdump files for ABI checking
        git -C "${REPO_ROOT_DIR}/build/soong" apply "${REPO_ROOT_DIR}/vendor/volvocars/tools/ci/jenkins/abi-dump.patch"
        # Restore file at exit/user break/external termination during builds
        trap 'git -C "${REPO_ROOT_DIR}/build/soong" checkout cc/library.go' EXIT SIGINT SIGTERM
    fi
fi

#shellcheck disable=SC2086
docker run \
    --rm \
    ${INTERACTIVE_OPTS} \
    ${ENV_FILE_OPT} \
    --hostname aic-docker \
    $VOLUMES \
    --dns=10.244.0.15 \
    --dns=10.244.0.20 \
    --dns-search=dhcp.nordic.volvocars.net \
    --env=HOST_UID="$(id -u)" \
    --env=HOST_GID="$(id -g)" \
    --env=HOST_UNAME="$(id -un)" \
    --env=HOST_HOSTNAME="$(hostname)" \
    --env CCACHE_DIR \
    --env CCACHE_MAXSIZE \
    --env CC_WRAPPER \
    --env CXX_WRAPPER \
    --env OUT_DIR \
    --env USE_CCACHE \
    --env SKIP_ABI_CHECKS \
    --env=HOST_USER_GROUPS="${HOST_USER_GROUPS}" \
    --env=REPO_ROOT_DIR="${REPO_ROOT_DIR}" \
    --env=HOME="$HOME" \
    --workdir "${WORKING_DIR}" \
    --privileged \
    --net=host \
    --env DISPLAY \
    "${DOCKER_IMAGE}" \
    "$@"

RETVAL=$?

if [[ $IS_TERMINAL == true && $IS_INTERACTIVE == true ]]; then
    # Restore library.go
    if [ ! -z "${SKIP_ABI_CHECKS+x}" ]; then
        git -C "${REPO_ROOT_DIR}/build/soong" checkout cc/library.go
    fi
fi

exit $RETVAL