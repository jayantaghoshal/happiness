#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../..)

PRE_COMMIT_SCRIPT="${SCRIPT_DIR}/pre-commit_in-docker.sh"
echo "Running ${PRE_COMMIT_SCRIPT} in docker..."
"${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/run.sh" "${PRE_COMMIT_SCRIPT}" < /dev/null