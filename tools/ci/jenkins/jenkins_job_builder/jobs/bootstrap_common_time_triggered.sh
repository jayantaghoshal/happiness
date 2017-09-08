#!/bin/bash
set -x

die() { echo "$@" 1>&2 ; exit 1; }

##
# Remove old repo
#
build_dir=aosp

if [[ -d $build_dir ]]; then
  rm -rf $build_dir;
fi

mkdir $build_dir
cd $build_dir

##
# Configure Docker 
#
export DOCKER_IMAGE=swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build:If943907d331a19834bdfea658f72144a0e503a08
export WORKSPACE_ROOT
WORKSPACE_ROOT=$(pwd)

docker_run () {
    docker run \
    --hostname aic-docker \
    --volume "$WORKSPACE_ROOT":"$WORKSPACE_ROOT" \
    --volume "$HOME":"$HOME" \
    --env=HOST_UID="$(id -u)" \
    --env=HOST_GID="$(id -g)" \
    --env=HOST_UNAME="$(id -un)" \
    --env=REPO_ROOT_DIR="${WORKSPACE_ROOT}" \
    --env=HOME="$HOME" \
    --env BASE_LOG_PATH \
    --env LOG_PATH \
    --env WORKSPACE_ROOT \
    --workdir "${WORKSPACE_ROOT}" \
    "${DOCKER_IMAGE}" \
    "$@"
}

