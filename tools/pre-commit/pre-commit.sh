#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../..)

PRE_COMMIT_SCRIPT="${SCRIPT_DIR}/pre-commit_in-docker.sh"

if [ "$HOSTNAME" = "aic-docker" ]; then
    echo "Already in aic-docker, running ${PRE_COMMIT_SCRIPT}..."
    "${PRE_COMMIT_SCRIPT}"
    exit 0
else
    echo "Launching new aic-docker, running ${PRE_COMMIT_SCRIPT}..."
    "${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/run.sh" "${PRE_COMMIT_SCRIPT}" < /dev/null
    exit 0
fi
