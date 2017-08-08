#!/bin/bash
set -e
set +x

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd $SCRIPT_PATH > /dev/null

if [ $# -eq 0 ]; then
    echo "Usage: $0 <CHANGE ID>"
    exit 1
fi

CHANGE_ID=$1

# Now we want to double check that the user is doing the right thing :-)
CHANGE_ID_FROM_LOG=$(git log -n 1 | grep Change-Id: | sed -r 's/.*Change-Id: (.*)/\1/')
if [[ $CHANGE_ID != $CHANGE_ID_FROM_LOG ]]; then
  echo "ERROR! Change Id from latest commit (${CHANGE_ID_FROM_LOG}) did not match the one given on command line (${CHANGE_ID})."
  exit 1
fi

LOCAL_NAME=vcc_aosp_build
TAGGED_CHANGE_ID=$(docker inspect --format "{{ index .Config.Labels \"org.label-schema.vcs-ref\"}}" ${LOCAL_NAME})

# Now we want to tripple check that the user is doing the right thing :-)
if [[ $CHANGE_ID != $TAGGED_CHANGE_ID ]]; then
  echo "ERROR! Tagged Change Id (${TAGGED_CHANGE_ID}) from image ${LOCAL_NAME} does not match the one given on command line (${CHANGE_ID})."
  exit 1
fi

REGISTRY_HOST="swf1.artifactory.cm.volvocars.biz:5002"
REMOTE_NAME="${REGISTRY_HOST}/test/vcc_aosp_build:${TAGGED_CHANGE_ID}"

echo "Tagging docker image ${LOCAL_NAME} with ${REMOTE_NAME}"
docker tag ${LOCAL_NAME} ${REMOTE_NAME}

echo "Pushing docker image to Artifactory..."
docker push ${REMOTE_NAME}

popd > /dev/null