#!/bin/bash
# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project
set -ue

# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
pushd "$SCRIPT_PATH" > /dev/null

TIMESTAMP=$(date -u +"%Y-%m-%dT%H:%M:%S%z")
CHANGE_ID=$(git log -n 1 | grep Change-Id: | sed -r 's/.*Change-Id: (.*)/\1/')
IMAGE_NAME=vcc_aosp_build

#collect SWF1 username and password(API) for PYPI access
echo "Enter your credentials for accessing SWF1_PYPI"
read -p 'CDSID:' pypi_user
read -sp 'Password:' pypi_pass

docker build \
--build-arg swf1_pypi_user="${pypi_user}" --build-arg swf1_pypi_pass="${pypi_pass}" \
--label org.label-schema.vcs-ref="${CHANGE_ID}" --label org.label-schema.build-date="${TIMESTAMP}" -t "${IMAGE_NAME}" .

popd > /dev/null