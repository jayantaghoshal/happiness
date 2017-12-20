#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
set +x

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd "$SCRIPT_PATH" > /dev/null

if [ $# -eq 0 ]; then
    echo "Usage: $0 <CHANGE ID>"
    exit 1
fi

CHANGE_ID=$1

# Check that command line change id matches the one in current commit.
CHANGE_ID_FROM_LOG=$(git log -n 1 | grep Change-Id: | sed -r 's/.*Change-Id: (.*)/\1/')
if [[ "$CHANGE_ID" != "$CHANGE_ID_FROM_LOG" ]]; then
  echo "ERROR! Change Id from latest commit (${CHANGE_ID_FROM_LOG}) did not match the one given on command line (${CHANGE_ID})."
  exit 1
fi

# Check that command line change id matches image tag.
LOCAL_NAME=vcc_aosp_build
TAGGED_CHANGE_ID=$(docker inspect --format "{{ index .Config.Labels \"org.label-schema.vcs-ref\"}}" ${LOCAL_NAME})
if [[ $CHANGE_ID != $TAGGED_CHANGE_ID ]]; then
  echo "ERROR! Tagged Change Id (${TAGGED_CHANGE_ID}) from image ${LOCAL_NAME} does not match the one given on command line (${CHANGE_ID})."
  exit 1
fi

# Check that image.ref contains correct path to artifactory image.
REGISTRY_HOST="swf1.artifactory.cm.volvocars.biz:5002"
REMOTE_NAME="${REGISTRY_HOST}/test/vcc_aosp_build:${TAGGED_CHANGE_ID}"
IMAGE_REF_FILE=$SCRIPT_PATH/../image.ref
IMAGE_REF_FILE_STR=$(<"$IMAGE_REF_FILE")
if [[ $IMAGE_REF_FILE_STR != $REMOTE_NAME ]]; then
  echo "ERROR! You seemed to forgot to update file ${IMAGE_REF_FILE}. Found \"${IMAGE_REF_FILE_STR}\", but expected \"${REMOTE_NAME}\"."
  exit 1
fi

echo "Tagging docker image ${LOCAL_NAME} with ${REMOTE_NAME}"
docker tag "${LOCAL_NAME}" "${REMOTE_NAME}"

echo "Pushing docker image to Artifactory..."
docker push "${REMOTE_NAME}"

popd > /dev/null