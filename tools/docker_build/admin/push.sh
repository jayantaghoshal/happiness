#!/bin/bash
set -e
set +x

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd $SCRIPT_PATH > /dev/null

if [ $# -eq 0 ]; then
    echo "Usage: $0 <IMAGE ID>"
    exit 1
fi

IMAGE_ID=$1
COMMIT=`docker inspect --format "{{ index .Config.Labels \"org.label-schema.vcs-ref\"}}" ${IMAGE_ID}`
REGISTRY_HOST="swf1.artifactory.cm.volvocars.biz:5002"
NAME="${REGISTRY_HOST}/test/vcc_aosp_build:${COMMIT}"

echo "Tagging docker image ${IMAGE_ID}..."
docker tag ${IMAGE_ID} ${NAME}
echo "New name: ${NAME}"

echo "Pushing docker image to Artifactory..."
docker push ${NAME}

popd > /dev/null