#!/bin/bash

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd "$SCRIPT_PATH" > /dev/null

TIMESTAMP=$(date -u +"%Y-%m-%dT%H:%M:%S%z")
CHANGE_ID=$(git log -n 1 | grep Change-Id: | sed -r 's/.*Change-Id: (.*)/\1/')
IMAGE_NAME=vcc_aosp_build

docker build --label org.label-schema.vcs-ref="${CHANGE_ID}" --label org.label-schema.build-date="${TIMESTAMP}" -t "${IMAGE_NAME}" .

popd > /dev/null