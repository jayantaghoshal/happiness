#!/bin/bash

IMAGE_NAME=aic_build

HOST_CCACHE_DIR=$HOME/aic-docker-ccache
if ! test -e ${HOST_CCACHE_DIR}; then
  echo "Creating directory ${HOST_CCACHE_DIR}..."
  mkdir -p ${HOST_CCACHE_DIR}
fi

docker run \
    --tty \
    --interactive \
    --hostname aic-docker \
    --volume $PWD:$PWD \
    --env "LOCAL_USER_ID=`stat -c %u $PWD`" \
    --env "LOCAL_GRP_ID=`stat -c %g $PWD`" \
    --env "USE_CCACHE=1" \
    --env "CCACHE_DIR=${HOST_CCACHE_DIR}" \
    --volume ${HOST_CCACHE_DIR}:${HOST_CCACHE_DIR} \
    ${IMAGE_NAME} \
    $PWD "./prebuilts/misc/linux-x86/ccache/ccache -M 10G && $*"
