#!/bin/bash

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd $SCRIPT_PATH > /dev/null

TIMESTAMP=`date -u +"%Y-%m-%dT%H:%M:%S%z"`
COMMIT=`git rev-parse --verify HEAD`
IMAGE_NAME=vcc_aosp_build

docker build --label org.label-schema.vcs-ref=${COMMIT} --label org.label-schema.build-date=${TIMESTAMP} -t ${IMAGE_NAME} .

popd > /dev/null