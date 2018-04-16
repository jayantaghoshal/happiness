#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e

WORKING_DIR="$(pwd)"
SCRIPT_DIR="$(cd "$(dirname "$(readlink -f "$0")")"; pwd)"
REPO_ROOT_DIR="$(readlink -f "${SCRIPT_DIR}/../../../..")"

DOCKER_IMAGE_REFERENCE_FILE="${SCRIPT_DIR}"/image.ref
DOCKER_IMAGE="$(cat "${DOCKER_IMAGE_REFERENCE_FILE}")"

# prune is always disabled, enabled by flag
OPT_RUN_DOCKER_SYSTEM_PRUNE=false
THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


function _docker_system_prune()
{
    # we would like to run a cleanup in the docker
    # environment, we ensure that the latest image
    # won't be deleted, everything else is wiped
    build_latest_img=$(python "${THIS_DIR}/get_image_rev.py" -I "${THIS_DIR}/image.ref")
    #TODO: Remove || true once we upgrade all NUCs to Docker 17.04 or later
    docker system prune --force --filter "label!=org.label-schema.vcs-ref=${build_latest_img}" || true
    echo 'Cleaning docker engine'
}


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
VOLUMES="--volume=$HOME:$HOME --volume=$REPO_ROOT_DIR:$REPO_ROOT_DIR --volume=$PWD:$PWD \
         --volume=/dev/bus/usb:/dev/bus/usb"


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
    --docker-prune)
    OPT_RUN_DOCKER_SYSTEM_PRUNE=true
    # ensure that we consume the argument
    # docker command doesn't understand it
    shift
    ;;
    *)
            # unknown option
    ;;
esac
done

if ${OPT_RUN_DOCKER_SYSTEM_PRUNE}; then
    _docker_system_prune
fi


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
        #Don't do MIRROR_HOST_USER_AND_GROUPS, because we are reattching to a container that already did so
        docker exec --tty --interactive --env MIRROR_HOST_USER_AND_GROUPS=0 "${EXISTING_CONTAINER_ARRAY[0]}" /tmp/entrypoint.sh bash
        exit 0
    fi
    echo -e "${C_OFF}"
fi



# Default to using ccache
# Technically unnecessary (the build system only checks for "not false") but explicit for clarity
if [[ ! -v USE_CCACHE ]]; then
  export USE_CCACHE=true
fi

# Get a comma-separated list of group ids for the host user groups
# Used by useradd inside entrypoint
HOST_USER_GROUPS=$(id -G "$USER")
HOST_USER_GROUPS_COMMA_SEPARATED=${HOST_USER_GROUPS// /,}


INTERACTIVE_OPTS=""
if [[ $IS_INTERACTIVE == true ]]; then
    INTERACTIVE_OPTS="${INTERACTIVE_OPTS} --interactive "
fi
if [[ $IS_TERMINAL == true ]]; then
    INTERACTIVE_OPTS="${INTERACTIVE_OPTS} --tty "
fi

#shellcheck disable=SC2086
docker run \
    --rm \
    ${INTERACTIVE_OPTS} \
    ${ENV_FILE_OPT} \
    --hostname aic-docker \
    --add-host aic-docker:127.0.0.1 \
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
    --env=HOST_USER_GROUPS_COMMA_SEPARATED="${HOST_USER_GROUPS_COMMA_SEPARATED}" \
    --env=REPO_ROOT_DIR="${REPO_ROOT_DIR}" \
    --env=HOME="$HOME" \
    --env=MIRROR_HOST_USER_AND_GROUPS=1 \
    --workdir "${WORKING_DIR}" \
    --privileged \
    --net=host \
    --env DISPLAY \
    "${DOCKER_IMAGE}" \
    "$@"

RETVAL=$?

exit $RETVAL
