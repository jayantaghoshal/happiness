#!/bin/bash

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd $SCRIPT_PATH > /dev/null


IMAGE_NAME=vcc_aosp_build

docker build -t ${IMAGE_NAME} .

popd > /dev/null